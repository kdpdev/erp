#include <erp/department.h>
#include <erp/erp_utils.h>

namespace Erp
{
  namespace
  {
    bool CheckNotEmpty(const Department& department, const TaskResultListener& listener)
    {
      if (department.Empty())
      {
        listener(TaskResult::MakeFailed("Department is empty."));
        return false;
      }
      return true;
    }

    TaskResult PersonalizeDetails(const Employee& employee, const TaskResult& taskResult)
    {
      return TaskResult(
        taskResult.status,
        employee.GetSpecialization() + " : " + employee.GetName() + " : " + taskResult.details);
    }
  }

  Department::Department(const std::string& name)
    : m_name(name)
  {
  }

  std::string Department::GetName() const
  {
    return m_name;
  }

  void Department::SetReglament(ReglamentPtr reglament)
  {
    m_reglament = std::move(reglament);
    EnumAllObjs([this](auto& obj)
    {
      obj.SetReglament(m_reglament);
    });
  }

  void Department::OnObjAdded(Obj& obj)
  {
    obj.SetReglament(m_reglament);
  }

  void Department::Exec(const TaskInfo& task, const TaskResultListener& listener)
  {
    if (CheckNotEmpty(*this, listener))
    {
      bool executed = false;
      EnumAllObjs([this, &task, &listener, &executed](Employee& employee)
      {
        if (m_reglament->CanExec(employee.GetSpecialization(), task))
        {
          executed = true;
          listener(PersonalizeDetails(employee, employee.Exec(task)));
        }
      });

      if (!executed)
      {
        listener(TaskResult::MakeFailed("No specialised employees."));
      }
    }
  }

  void Department::Exec(const TaskInfo& task, const std::string& employeeName, const TaskResultListener& listener)
  {
    if (CheckNotEmpty(*this, listener))
    {
      bool executed = false;
      EnumAllObjs([&task, &employeeName, &listener, &executed](Employee& employee)
      {
        if (employee.GetName() == employeeName)
        {
          executed = true;
          listener(PersonalizeDetails(employee, employee.Exec(task)));
        }
      });

      if (!executed)
      {
        listener(TaskResult::MakeFailed("No employee with the '" + employeeName + "' name."));
      }
    }
  }
}
