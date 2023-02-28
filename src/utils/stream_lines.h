#pragma once

#include <utils/lines.h>

#include <iosfwd>
#include <memory>

namespace Utils
{
  std::unique_ptr<LinesIter> CreateLoggingLinesIter(
    const std::string& logPrefix,
    std::ostream& log,
    std::unique_ptr<LinesIter> linesIter);

  std::unique_ptr<LinesIter> CreateLoggingLinesIter(
    const std::string& logPrefix,
    std::ostream& log,
    std::unique_ptr<LinesIter> linesIter);


  std::unique_ptr<LinesIter> CreateStreamLinesIter(
    std::istream& stream);

  std::unique_ptr<LinesIter> CreateStreamLinesIter(
    std::unique_ptr<std::istream> stream);


  std::unique_ptr<LinesIter> CreateConsoleLinesIter(
    const std::string& inputPrefix);
}
