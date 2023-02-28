#include <erp/erp_utils.h>
#include <erp/employee.h>
#include <erp/department.h>
#include <erp/enterprise.h>
#include <utils/common.h>
#include <utils/stream_lines.h>
#include <utils/tokenizers.h>

#include <iostream>
#include <sstream>
#include <unordered_map>

namespace Erp
{
  std::string ToHumanString(const Erp::TaskResult::Status obj)
  {
    using Status = TaskResult::Status;
    using StatusType = std::underlying_type_t<Status>;
    using StatusNames = std::unordered_map<StatusType, std::string>;
    static const StatusNames statusNames =
    {
      { static_cast<StatusType>(Status::IDLE)       , "IDLE" },
      { static_cast<StatusType>(Status::SUCCESSED)  , "SUCCESSED" },
      { static_cast<StatusType>(Status::FAILED)     , "FAILED" }
    };

    const auto iter = statusNames.find(static_cast<StatusType>(obj));
    if (iter != statusNames.end())
    {
      return iter->second;
    }

    return "Unknown(" + std::to_string(static_cast<StatusType>(obj)) + ")";
  }

  std::string ToHumanString(const Erp::TaskResult& obj)
  {
    std::ostringstream oss;
    oss << "Status:  " << ToHumanString(obj.status) << std::endl;
    oss << "Details: " << obj.details << std::endl;
    return oss.str();
  }

  std::string ToHumanString(const Erp::Employee& obj)
  {
    std::ostringstream oss;
    oss << "Employee";
    oss << " : " << obj.GetName();
    oss << " : " << obj.GetSpecialization();
    return oss.str();
  }

  std::string ToHumanString(const Erp::Department& obj)
  {
    std::ostringstream oss;
    oss << "Department";
    oss << " : " << obj.GetName();
    oss << " : [" << obj.GetObjsCount() << "]";
    for (const auto& empl : obj.GetObjs())
    {
      oss << std::endl << "  " << ToHumanString(empl);
    }
    return oss.str();
  }

  std::string ToHumanString(const Erp::Enterprise& obj)
  {
    const auto reglament = obj.GetReglament();
    std::ostringstream oss;
    oss << "Enterprise" << std::endl;
    oss << (reglament == nullptr ? std::string("Reglament: null") : reglament->GetDescription()) << std::endl;
    oss << "Departments : [" << obj.GetObjsCount() << "]" << std::endl;
    for (const auto& dep : obj.GetObjs())
    {
      oss << "  " << dep.GetName() << " [" << dep.GetObjsCount() << "]" << std::endl;
    }

    const auto separator = "----------------------";
    for (const auto& dep : obj.GetObjs())
    {
      oss << separator << std::endl;
      oss << ToHumanString(dep) << std::endl;
    }
    oss << separator;

    return oss.str();
  }

  TaskResultListener PrependDetails(const std::string& str, TaskResultListener listener)
  {
    if (str.empty() || !listener)
    {
      return listener;
    }
    return[str, listener = std::move(listener)](const TaskResult& result)
    {
      listener(TaskResult(result.status, str + " : " + result.details));
    };
  }

  std::unique_ptr<Reglament> CreateEmptyReglament()
  {
    class EmptyReglament : public Reglament
    {
    public:
      virtual std::string GetDescription() const override
      {
        return "Reglament";
      }

      virtual bool CanExec(const Specialization&, const TaskType&) const override
      {
        return false;
      }
    };
    return std::make_unique<EmptyReglament>();
  }

  std::unique_ptr<Reglament> CreateDefaultReglament(
    const std::set<TaskType>& common,
    const std::map<Reglament::Specialization, std::set<TaskType>>& specialized)
  {
    class DefaultReglament : public Reglament
    {
      const std::set<TaskType> m_common;
      const std::map<Reglament::Specialization, std::set<TaskType>> m_specialized;

    public:
      DefaultReglament(
        const std::set<TaskType>& common,
        const std::map<Reglament::Specialization, std::set<TaskType>>& specialized)
        : m_common(common)
        , m_specialized(specialized)
      {
      }

      virtual std::string GetDescription() const override
      {
        std::ostringstream oss;
        oss << "Reglament: ";
        oss << std::endl << "  Common:";
        for (const auto& entry : m_common)
        {
          oss << std::endl << "    " << entry;
        }
        oss << std::endl << "  Specialized:";
        for (const auto& entry : m_specialized)
        {
          oss << std::endl << "    " << entry.first << ":";
          for (const auto& task : entry.second)
          {
            oss << std::endl << "      " << task;
          }
        }
        return oss.str();
      }

      virtual bool CanExec(const Specialization& spec, const TaskType& taskType) const override
      {
        if (m_common.find(taskType) != m_common.end())
        {
          return true;
        }

        const auto it = m_specialized.find(spec);
        if (it != m_specialized.end())
        {
          return it->second.find(taskType) != it->second.end();
        }

        return false;
      }
    };

    return std::unique_ptr<Reglament>(new DefaultReglament(common, specialized));
    //return std::make_unique<DefaultReglament>(common, specialized); // MSVS errors
  }

  Department CreateDepartmentFromCsvStream(
    const std::string& name,
    std::istream& csvStream)
  {
    auto createEmployee = [](const std::string& csvLine)
    {
      const auto tokens = Utils::TokenizeCsvLine(csvLine);
      const bool isLineValid = tokens.size() == 2
        && !tokens[0].empty()
        && !tokens[1].empty();
      Utils::CheckArgTrue(isLineValid, "csvLine: '" + csvLine + "'");
      return Erp::Employee(tokens.at(1), tokens.at(0));
    };

    Erp::Department department(name);
    const auto linesIter = Utils::CreateStreamLinesIter(csvStream);
    Utils::Enumerate(*linesIter, [&department, &createEmployee](const std::string& line)
    {
      department.AddObj(createEmployee(line));
      return true;
    });
    return department;
  }

  std::unique_ptr<Enterprise> CreateEnterpriseFromDepartmentCsvStreams(
    const std::map<std::string, std::unique_ptr<std::istream>>& streams)
  {
    auto enterprise = std::make_unique<Erp::Enterprise>();
    for (auto& entry : streams)
    {
      Utils::CheckArgTrue(entry.second != nullptr, entry.first);
      enterprise->AddObj(CreateDepartmentFromCsvStream(entry.first, *entry.second));
    }
    return enterprise;
  }
}
