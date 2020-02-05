#include <iomanip>
#include "log.h"

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

Event::Information::Information(time_t time,
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

Layout::Layout(std::string pattern)
        : m_pattern(std::move(pattern))
{
}

std::string Layout::formatter(Level level, const Event::Information& information)
{
    // TODO: 根据 pattern 生成结果

    std::ostringstream stream;
    stream << "[" << std::put_time(std::localtime(&information.m_time), "%Y-%m-%d %H:%M:%S") << "] [" << information.m_threadId << ":" << information.m_threadName << "] ["
           << information.m_fileName << ":" << information.m_funcName << ":" << information.m_line << "] ["
           << ToString(level) << "] " << information.m_stream.str();
    return stream.str();
}

// ---------------------------------------------------------------------------------------------------------------------

Appender::Appender(std::shared_ptr<Layout> layout)
        : m_layout(std::move(layout))
{
}

ConsoleAppender::ConsoleAppender(std::shared_ptr<Layout> layout)
        : Appender(std::move(layout))
{
}

void ConsoleAppender::write(Level level, const Event::Information& information)
{
    std::cout << m_layout->formatter(level, information) << std::endl;
}

FileAppender::FileAppender(std::string fileName, std::shared_ptr<Layout> layout)
        : Appender(std::move(layout)), m_fileName(std::move(fileName))
{
    m_ofstream.open(m_fileName, std::ofstream::out | std::ofstream::app);
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

Logger::Logger(Level level)
        : baseline(level)
{
    addAppender(
        std::make_shared<ConsoleAppender>(
            std::make_shared<Layout>()
    ));
}

std::shared_ptr<Logger> Logger::instance(new Logger());

std::shared_ptr<Logger> Logger::getInstance()
{
    return instance;
}

void Logger::log(Level level, const Event::Information& information)
{
    // 当 logger 的 baseline 为 OFF 时，不输出该日志
    if (baseline == Level::OFF)
        return;
    // 当 level 小于 logger 的 baseline 时，不输出该日志
    if (level < baseline)
        return;
    // 当 logger 的 baseline 为 UNKNOWN 或 ALL 时，输出该日志
    // 当 level 大于等于 logger 的 baseline 时，输出该日志
    for (const auto& appender : m_appenders)
        appender->write(level, information);
}

// ---------------------------------------------------------------------------------------------------------------------

}   // namespace log
}   // namespace o7si
