#pragma once

#include <erp/reglament.h>

#include <iosfwd>
#include <map>
#include <memory>
#include <set>

namespace Hardcode
{
  using CsvInStreamsMap = std::map<std::string, std::unique_ptr<std::istream>>;

  std::unique_ptr<Erp::Reglament> GetReglament();
  CsvInStreamsMap GetDepartmentCvsStreams();
  std::unique_ptr<std::istream> GetCommandsStream();
}
