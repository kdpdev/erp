#include "hardcode.h"
#include <erp/erp_utils.h>

#include <sstream>

namespace Hardcode
{
  std::unique_ptr<Erp::Reglament> GetReglament()
  {
    const std::set<std::string> common =
    {
      "GoToVocation",
      "CleanWorkplace"
    };

    std::map<std::string, std::set<std::string>> specialised;
    specialised["Developer"]  = { "WriteCode", "DesignProject" };
    specialised["TechWriter"] = { "TranslateText" };
    specialised["Tester"]     = { "Test" };
    specialised["Accountant"] = { "ChargeMoney", "CreateReport" };

    return Erp::CreateDefaultReglament(common, specialised);
  }

  CsvInStreamsMap GetDepartmentCvsStreams()
  {
    CsvInStreamsMap streams;
    {
      auto ssPtr = std::make_unique<std::stringstream>();
      auto& oss = *ssPtr;
      oss << "Developer;Vova"    << std::endl;
      oss << "Developer;Petr"    << std::endl;
      oss << "Developer;Alex"    << std::endl;
      oss << "Developer;Tom"     << std::endl;
      oss << "Developer;Bill"    << std::endl;
      oss << "Tester;Vova"       << std::endl;
      oss << "Tester;Nina"       << std::endl;
      oss << "Tester;Nina"       << std::endl;
      oss << "TechWriter;Eugene" << std::endl;
      oss << "TechWriter;Ann"    << std::endl;
      oss << "TechWriter;Alex"   << std::endl;
      oss << "Accountant;Lena"   << std::endl;
      streams.emplace("Big", std::move(ssPtr));
    }

    {
      auto ssPtr = std::make_unique<std::stringstream>();
      auto& oss = *ssPtr;
      oss << "Developer;Vova" << std::endl;
      oss << "Developer;Dima" << std::endl;
      oss << "Developer;Tim" << std::endl;
      oss << "Tester;Vova" << std::endl;
      oss << "TechWriter;Ann" << std::endl;
      oss << "Accountant;Lena" << std::endl;
      streams.emplace("Medium", std::move(ssPtr));
    }

    {
      auto ssPtr = std::make_unique<std::stringstream>();
      auto& oss = *ssPtr;
      oss << "Tester;John"     << std::endl;
      oss << "TechWriter;Lena" << std::endl;
      streams.emplace("Small", std::move(ssPtr));
    }

    {
      auto ssPtr = std::make_unique<std::stringstream>();
      streams.emplace("Dummy", std::move(ssPtr));
    }

    return streams;
  }

  std::unique_ptr<std::istream> GetCommandsStream()
  {
    auto ssPtr = std::make_unique<std::stringstream>();
    auto& oss = *ssPtr;

    oss << "PrintStructure" << std::endl;
    oss << "GoToVocation" << std::endl;
    oss << "CleanWorkplace" << std::endl;
    oss << "WriteCode" << std::endl;
    oss << "DesignProject" << std::endl;
    oss << "Test" << std::endl;
    oss << "TranslateText" << std::endl;
    oss << "ChargeMoney" << std::endl;
    oss << "CreateReport" << std::endl;
    oss << "BadCommand" << std::endl;

    oss << "GoToVocation Department X" << std::endl;
    oss << "Test Department X" << std::endl;
    oss << "CleanWorkspace Department X Y" << std::endl;
    oss << "GoToVocation Department Small" << std::endl;
    oss << "CleanWorkplace Department Big" << std::endl;
    oss << "WriteCode Department Medium" << std::endl;
    oss << "DesignProject Department Small" << std::endl;
    oss << "Test Department Big" << std::endl;
    oss << "CreateReport Department Big" << std::endl;
    oss << "BadCommand Department Big" << std::endl;

    oss << "GoToVocation Employee X" << std::endl;
    oss << "GoToVocation Employee Vova" << std::endl;
    oss << "WriteCode Employee Lena" << std::endl;
    oss << "TranslateText Employee Vova" << std::endl;
    oss << "TranslateText Employee Lena" << std::endl;
    oss << "CreateReport Employee Lena" << std::endl;
    oss << "Test Employee Nina" << std::endl;
    oss << "DesignProject Employee Tom" << std::endl;

    return std::move(ssPtr);
  }
}
