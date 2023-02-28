#include <utils/stream_lines.h>
#include <utils/common.h>

#include <iostream>

namespace Utils
{
  namespace
  {
    class LoggingLinesIter : public LinesIter
    {
      const std::string m_logPrefix;
      const std::shared_ptr<std::ostream> m_log;
      const std::unique_ptr<LinesIter> m_iter;

    public:
      LoggingLinesIter(const std::string& logPrefix,
                       std::shared_ptr<std::ostream> log,
                       std::unique_ptr<LinesIter> iter)
        : m_logPrefix(logPrefix)
        , m_log(std::move(log))
        , m_iter(std::move(iter))
      {
        Utils::CheckArgTrue(m_log != nullptr, "log");
        Utils::CheckArgTrue(m_iter != nullptr, "iter");
      }

      virtual bool Next(std::string& line) override
      {
        std::string tmp;
        if (!m_iter->Next(tmp))
        {
          return false;
        }

        *m_log << m_logPrefix << tmp << std::endl;
        line = std::move(tmp);
        return true;
      }
    };


    class StreamLinesIter : public LinesIter
    {
    protected:
      std::shared_ptr<std::istream> m_in;

    public:
      explicit StreamLinesIter(std::shared_ptr<std::istream> in)
        : m_in(std::move(in))
      {
        Utils::CheckArgTrue(m_in != nullptr, "in");
      }

      virtual bool Next(std::string& line) override
      {
        if (!m_in)
        {
          return false;
        }

        auto& in = *m_in;
        in.clear();
        in.exceptions(std::ios::badbit);

        std::string tmp;
        std::getline(in, tmp);
        if (in.fail() && !in.eof())
        {
          in.exceptions(std::ios::badbit | std::ios::failbit);
        }
        if (in.eof())
        {
          m_in.reset();
          return false;
        }

        line = std::move(tmp);
        return true;
      }
    };


    class ConsoleLinesIter : public StreamLinesIter
    {
      std::ostream& m_out;
      const std::string m_linePrefix;

    public:
      explicit ConsoleLinesIter(const std::string& linePrefix)
        : StreamLinesIter(Utils::MakeNotDeletedShared(&std::cin))
        , m_out(std::cout)
        , m_linePrefix(linePrefix)
      {
      }

      virtual bool Next(std::string& line) override
      {
        if (!m_in)
        {
          return false;
        }

        m_out << m_linePrefix;
        return StreamLinesIter::Next(line);
      }
    };
  }

  std::unique_ptr<LinesIter> CreateLoggingLinesIter(
    const std::string& logPrefix,
    std::ostream& log,
    std::unique_ptr<LinesIter> linesIter)
  {
    return std::make_unique<LoggingLinesIter>(
      logPrefix,
      Utils::MakeNotDeletedShared(&log),
      std::move(linesIter));
  }

  std::unique_ptr<LinesIter> CreateLoggingLinesIter(
    const std::string& logPrefix,
    std::unique_ptr<std::ostream> log,
    std::unique_ptr<LinesIter> linesIter)
  {
    return std::make_unique<LoggingLinesIter>(
      logPrefix,
      std::move(log),
      std::move(linesIter));
  }

  std::unique_ptr<LinesIter> CreateStreamLinesIter(
    std::istream& stream)
  {
    return std::make_unique<StreamLinesIter>(Utils::MakeNotDeletedShared(&stream));
  }

  std::unique_ptr<LinesIter> CreateStreamLinesIter(
    std::unique_ptr<std::istream> stream)
  {
    return std::make_unique<StreamLinesIter>(std::move(stream));
  }

  std::unique_ptr<LinesIter> CreateConsoleLinesIter(
    const std::string& inputPrefix)
  {
    return std::make_unique<ConsoleLinesIter>(inputPrefix);
  }
}
