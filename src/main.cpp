#include "default_commands_handler.h"
#include "hardcode.h"
#include <erp/enterprise.h>
#include <erp/erp_utils.h>
#include <utils/common.h>
#include <utils/stream_lines.h>

#include <iostream>

namespace
{
  std::shared_ptr<Erp::Enterprise> CreateHardcodedEnterprise()
  {
    const auto departmentStreams = Hardcode::GetDepartmentCvsStreams();
    auto enterprise = Erp::CreateEnterpriseFromDepartmentCsvStreams(departmentStreams);
    enterprise->SetReglament(Hardcode::GetReglament());
    return std::move(enterprise);
  }

  auto CreateCommandsHandler(std::shared_ptr<Erp::Enterprise> enterprise)
  {
    const auto enterpriseCommandsHandler = CreateDefaultCommandsHandler(enterprise);
    return [enterpriseCommandsHandler](const std::string& command)
    {
      if (command == "exit")
      {
        return false;
      }
      Utils::CallWithTryCatch("Enterprise handler", enterpriseCommandsHandler, command);
      return true;
    };
  }

  void Exec()
  {
    const auto enterprise = CreateHardcodedEnterprise();
    const auto commandsHandler = CreateCommandsHandler(enterprise);

    // Hardcoded commands.
    //if (false)
    {
      const auto commandsPrefix = "###########################\nhardcode: ";
      auto commandsStream = Hardcode::GetCommandsStream();
      auto streamLinesIter = Utils::CreateStreamLinesIter(std::move(commandsStream));
      const auto commandsIter = Utils::CreateLoggingLinesIter(commandsPrefix, std::cout, std::move(streamLinesIter));
      Utils::Enumerate(*commandsIter, commandsHandler);
    }
    
    // Console commands
    //if (false)
    {
      commandsHandler("?");
      const auto commandsPrefix = "###########################\ncommand: ";
      const auto commandsIter = Utils::CreateConsoleLinesIter(commandsPrefix);
      Utils::Enumerate(*commandsIter, commandsHandler);
    }
  }
}

int main()
{
  const auto err = Utils::CallWithTryCatch("Exec", Exec);
  return err ? 1 : 0;
}
