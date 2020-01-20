#include "log.h"

#include <utility>

namespace o7si
{
namespace log
{

const char* ToString(Level level)
{
    switch (level)
    {
        case Level::ALL:
            return "ALL";
        case Level::DEBUG:
            return "DEBUG";
        case Level::INFO:
            return "INFO";
        case Level::WARN:
            return "WARN";
        case Level::ERROR:
            return "ERROR";
        case Level::FATAL:
            return "FATAL";
        case Level::OFF:
            return "OFF";
        default:
            return "UNKNOWN";
    }
}

Level FromString(const std::string& level)
{
    if (level == "ALL")
        return Level::ALL;
    if (level == "DEBUG")
        return Level::DEBUG;
    if (level == "INFO")
        return Level::INFO;
    if (level == "WARN")
        return Level::WARN;
    if (level == "ERROR")
        return Level::ERROR;
    if (level == "FATAL")
        return Level::FATAL;
    if (level == "OFF")
        return Level::OFF;
    return Level::UNKNOWN;
}


Logger::Information::Information(Level level, uint64_t time,
                                 uint32_t threadId, std::string threadName,
                                 std::string fileName, std::string funcName, uint32_t line)
        : m_level(level), m_time(time),
          m_threadId(threadId), m_threadName(std::move(threadName)),
          m_fileName(std::move(fileName)), m_funcName(std::move(funcName)), m_line(line)
{
}

Logger::Information::Information()
        : Information(Level::UNKNOWN, - 1, - 1, "", "", "", - 1)
{
}

std::shared_ptr<Logger> Logger::instance(new Logger());

std::shared_ptr<Logger> Logger::getInstance()
{
    return instance;
}

}   // namespace log
}   // namespace o7si