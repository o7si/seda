#include "log.h"

namespace o7si
{
namespace log
{

// ---------------------------------------------------------------------------------------------------------------------

Level GenLevelFrom(const std::string& level)
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

std::ostream& operator<<(std::ostream& stream, const Level& level)
{
    switch (level)
    {
        case Level::ALL:
            return stream << "ALL";
        case Level::DEBUG:
            return stream << "DEBUG";
        case Level::INFO:
            return stream << "INFO";
        case Level::WARN:
            return stream << "WARN";
        case Level::ERROR:
            return stream << "ERROR";
        case Level::FATAL:
            return stream << "FATAL";
        case Level::OFF:
            return stream << "OFF";
        default:
            return stream << "UNKNOWN";
    }
}

// ---------------------------------------------------------------------------------------------------------------------

Event::Information::Information(time_t time,
                                uint32_t threadId, std::string threadName,
                                std::string fileName, std::string funcName, uint32_t line,
                                std::string user)
        : m_time(time),
          m_threadId(threadId), m_threadName(std::move(threadName)),
          m_fileName(std::move(fileName)), m_funcName(std::move(funcName)), m_line(line),
          m_user(std::move(user))
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
    // 根据 pattern 生成结果

    std::ostringstream stream;
    stream << "[" << std::put_time(std::localtime(&information.m_time), "%Y-%m-%d %H:%M:%S") << "]"
           << "[" << information.m_threadId << ":" << information.m_threadName << "]"
           << "[" << information.m_fileName << ":" << information.m_funcName << ":" << information.m_line << "]"
           << "[" << level << "]" 
           << "[" << information.m_user << "]"
           << " "
           << information.m_stream.str();
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

FileAppender::FileAppender(std::string filename, std::shared_ptr<Layout> layout)
        : Appender(std::move(layout)), m_filename(std::move(filename))
{
    m_ofstream.open(m_filename, std::ofstream::out | std::ofstream::app);
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

Logger::Logger(Level baseline)
        : m_baseline(baseline)
{
}

Logger::~Logger()
{
    // 清空日志输出地
    clear_appenders();
} 

void Logger::log(Level level, const Event::Information& information)
{
    // 当 logger 的 baseline 为 OFF 时，不输出该日志
    if (m_baseline == Level::OFF)
        return;
    // 当 level 小于 logger 的 baseline 时，不输出该日志
    if (level < m_baseline)
        return;
    // 当 logger 的 baseline 为 UNKNOWN 或 ALL 时，输出该日志
    // 当 level 大于等于 logger 的 baseline 时，输出该日志
    for (const auto& appender : m_appenders)
        appender->write(level, information);
}

// ---------------------------------------------------------------------------------------------------------------------

std::shared_ptr<LoggerManager> LoggerManager::instance(new LoggerManager());

std::shared_ptr<LoggerManager> LoggerManager::Instance() 
{
    return instance;    
}

LoggerManager::LoggerManager()
{
    // 默认有一个系统用户，日志级别为 INFO
    m_table["sys"] = std::make_shared<Logger>(Level::INFO);    
    // 给系统用户添加一个默认的控制台输出地
    m_table["sys"]->add_appender(
        std::make_shared<ConsoleAppender>(
            std::make_shared<Layout>()
        )
    );
}

std::shared_ptr<Logger> LoggerManager::doRegister(const std::string& user, Level level)
{
    /// 用户名一概小写
    std::string lower_user = o7si::utils::to_lower(user);
    // 注册一个新的用户
    // 如果用户已经被注册，则什么都不做
    // 如果用户未被注册，则进行注册
    if (m_table.find(lower_user) == m_table.end())
        m_table[lower_user] = std::make_shared<Logger>(level);

    return m_table[lower_user]; 
}

std::shared_ptr<Logger> LoggerManager::doLogin(const std::string& user)
{
    /// 用户名一概小写
    std::string lower_user = o7si::utils::to_lower(user);
    // 登录
    // 如果登录用户已经注册，则什么都不做
    // 如果登录用户未被注册，则帮助其进行注册
    if (m_table.find(lower_user) == m_table.end())
        doRegister(lower_user, Level::DEBUG);

    return m_table[lower_user];
}

// ---------------------------------------------------------------------------------------------------------------------

}   // namespace log
}   // namespace o7si
