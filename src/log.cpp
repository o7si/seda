#include "log.h"

#include <utility>

namespace o7si
{
namespace log
{

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

std::shared_ptr<Logger> Logger::instance(new Logger());

std::shared_ptr<Logger> Logger::getInstance()
{
    return instance;
}

void Logger::log(Level level, const Event::Information& information)
{
    for (const auto& appender : m_appenders)
    {
        appender->write(level, information);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

Event::Information::Information(uint64_t time,
                                uint32_t threadId, std::string threadName,
                                std::string fileName, std::string funcName, uint32_t line)
        : m_time(time),
          m_threadId(threadId), m_threadName(std::move(threadName)),
          m_fileName(std::move(fileName)), m_funcName(std::move(funcName)), m_line(line)
{
}

Event::Event(std::shared_ptr<Logger> logger, Level level, Event::Information information)
        : m_logger(std::move(logger)), m_level(level), m_information(std::move(information))
{
}

Event::~Event()
{
    m_logger->log(m_level, m_information);
}

// ---------------------------------------------------------------------------------------------------------------------

void ConsoleAppender::write(Level level, const Event::Information& information)
{
    std::cout << m_layout->formatter(level, information) << std::endl;
}

FileAppender::FileAppender(std::string fileName)
        : m_fileName(std::move(fileName))
{
    m_ofstream.open(m_fileName);
}

FileAppender::~FileAppender()
{
    m_ofstream.close();
}

void FileAppender::write(Level level, const Event::Information& information)
{
    m_ofstream << m_layout->formatter(level, information) << std::endl;
}

// ---------------------------------------------------------------------------------------------------------------------

std::string Layout::formatter(Level level, const Event::Information& information)
{
    std::ostringstream stream;
    stream << "[" << information.m_time << "] [" << information.m_threadId << ":" << information.m_threadName << "] ["
           << information.m_fileName << ":" << information.m_funcName << ":" << information.m_line << "] ["
           << ToString(level) << "]";
    return stream.str();
}

// ---------------------------------------------------------------------------------------------------------------------

}   // namespace log
}   // namespace o7si