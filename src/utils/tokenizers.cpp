#include <utils/tokenizers.h>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

namespace Utils
{
  std::vector<std::string> TokenizeCsvLine(const std::string& line)
  {
    auto escapes = boost::escaped_list_separator<char>('\\', ';', '\"');
    auto tokens = boost::tokenizer<decltype(escapes)>(line, escapes);
    return std::vector<std::string>(tokens.begin(), tokens.end());
  }

  std::vector<std::string> TokenizeCommandLine(const std::string& line)
  {
    auto trimmedLine = line;
    boost::trim(trimmedLine);
    auto escapes = boost::escaped_list_separator<char>('\\', ' ', '\"');
    auto tokens = boost::tokenizer<decltype(escapes)>(trimmedLine, escapes);
    return std::vector<std::string>(tokens.begin(), tokens.end());
  }
}
