#include "default_commands_handler.h"
#include <erp/enterprise.h>
#include <erp/erp_utils.h>
#include <utils/common.h>
#include <utils/tokenizers.h>

#include <iostream>

namespace
{
  void PrintUsage()
  {
    std::cout << "Usage:" << std::endl;
    std::cout << "  ?                                  - print usage." << std::endl;
    std::cout << "  PrintStructure                     - print structure of the enterprise." << std::endl;
    std::cout << "  {Task}                             - attach task to whole enterprise." << std::endl;
    std::cout << "  {Task} Department {DepartmentName} - attach task to departments with the specified name." << std::endl;
    std::cout << "  {Task} Employee {EmployeeName}     - attach task to employees with the specified name." << std::endl;
  }

  template <typename Obj>
  void PrintHumanString(const Obj& obj)
  {
    std::cout << Erp::ToHumanString(obj) << std::endl;
  }

  void PrintTaskResult(const Erp::TaskResult& taskResult)
  {
    std::cout << "Result:" << std::endl;
    std::cout << Erp::ToHumanString(taskResult) << std::endl;
  }

  void HandleCommandLine(Erp::Enterprise& enterprise, const std::string& command)
  {
    Utils::CheckArgTrue(!command.empty(), "command");

    const auto tokens = Utils::TokenizeCommandLine(command);

    if (tokens.empty())
    {
      throw std::logic_error("Empty command.");
    }

    if (!(tokens.size() == 1 || tokens.size() == 3))
    {
      throw std::logic_error("Bad command format: '" + command + '\'');
    }

    const auto taskResultHandler = &PrintTaskResult;
    Erp::TaskInfo task;
    task.type = tokens.at(0);

    if (tokens.size() == 1)
    {
      if (task.type == "?")
      {
        PrintUsage();
        return;
      }

      if (task.type == "PrintStructure")
      {
        PrintHumanString(enterprise);
        return;
      }

      enterprise.ExecGlobal(task, taskResultHandler);
      return;
    }

    const auto& targetType = tokens.at(1);
    const auto& targetName = tokens.at(2);
    if (targetType == "Department")
    {
      enterprise.ExecDepartment(task, targetName, taskResultHandler);
      return;
    }

    if (targetType == "Employee")
    {
      enterprise.ExecEmployee(task, targetName, taskResultHandler);
      return;
    }

    throw std::logic_error("Bad command: unexpected targetType = '" + targetType + '\'');
  }
}

CommandsHandler CreateDefaultCommandsHandler(std::shared_ptr<Erp::Enterprise> enterprise)
{
  Utils::CheckArgTrue(enterprise != nullptr, "enterprise");
  return [enterprise = std::move(enterprise)](const std::string& command)
  {
    HandleCommandLine(*enterprise, command);
  };
}
