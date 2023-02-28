#pragma once

#include <string>

namespace Utils
{
  class LinesIter
  {
  public:
    virtual ~LinesIter() = default;
    virtual bool Next(std::string& line) = 0;
  };

  template <typename Consumer>
  void Enumerate(LinesIter& linesIter, const Consumer& consumer)
  {
    for (;;)
    {
      std::string str;
      auto ok = linesIter.Next(str);
      if (ok)
      {
        ok = consumer(str);
      }
      if (!ok)
      {
        break;
      }
    }
  }
}
