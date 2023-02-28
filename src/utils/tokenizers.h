#pragma once

#include <string>
#include <vector>

namespace Utils
{
  std::vector<std::string> TokenizeCsvLine(const std::string& line);
  std::vector<std::string> TokenizeCommandLine(const std::string& line);
}
