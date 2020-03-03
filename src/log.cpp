#include "log.h"


namespace o7si
{
namespace log
{

// ----------------------------------------------------------------------------

Level GenLevelFrom(const std::string& str)
{ 
    std::string upper = o7si::utils::to_upper(str);
#define XX(from, to) \
    if (upper == from) \
        return Level::to;
    XX("ALL", ALL)
    XX("DEBUG", DEBUG)
    XX("INFO", INFO)
    XX("WARN", WARN)
    XX("ERROR", ERROR)
    XX("FATAL", FATAL)
    XX("OFF", OFF)
#undef XX
    return Level::UNKNOWN;
}

Level GenLevelFrom(size_t num)
{
#define XX(from, to) \
    if (num == from) \
        return Level::to;
    XX(1, ALL)
    XX(2, DEBUG)
    XX(3, INFO)
    XX(4, WARN)
    XX(5, ERROR)
    XX(6, FATAL)
    XX(7, OFF)
#undef XX
    return Level::UNKNOWN;
}

std::ostream& operator<<(std::ostream& stream, const Level& level)
{
    switch (level)
    {
#define XX(from, to) \
        case Level::from: \
            return stream << to;
        XX(ALL, "ALL")
        XX(DEBUG, "DEBUG")
        XX(INFO, "INFO")
        XX(WARN, "WARN")
        XX(ERROR, "ERROR")
        XX(FATAL, "FATAL")
        XX(OFF, "OFF")
#undef XX
        default:
            return stream << "UNKNOWN";
    }
    return stream << "UNKNOWN";
}

// ----------------------------------------------------------------------------

Event::Information::Information(time_t time,
                                uint32_t threadId, std::string threadName,
                                std::string fileName, 
                                std::string funcName, uint32_t line,
                                std::string user)
        : m_time(time),
          m_threadId(threadId), m_threadName(std::move(threadName)),
          m_fileName(std::move(fileName)), 
          m_funcName(std::move(funcName)), m_line(line),
          m_user(std::move(user))
{
}

Event::Event(std::shared_ptr<Logger> logger, Level level, 
             Event::Information information, bool condition)
        : m_logger(std::move(logger)), m_level(level), 
          m_information(std::move(information)), m_condition(condition)
{
}

Event::~Event()
{
    // 当条件为真时才记录日志
    if (!m_condition)
        return;
    m_logger->log(m_level, m_information);
}

// ----------------------------------------------------------------------------

Layout::Layout(std::string pattern)
        : m_pattern(std::move(pattern))
{
}

std::string Layout::formatter(Level level, 
                              const Event::Information& information)
{
    // 说明使用者指定了输出格式
    if (!m_pattern.empty())
    {
        // 暂不支持
        // TODO：支持格式设定
        return "";    
    }

    // 当未指定输出格式时，输出默认格式
    std::ostringstream stream;
    stream << "[" << std::put_time(std::localtime(&information.m_time), 
                                   "%Y-%m-%d %H:%M:%S") << "]"
           << "[" << information.m_threadId << ":" 
           << information.m_threadName << "]"
           << "[" << information.m_fileName << ":" 
           << information.m_funcName << ":" << information.m_line << "]"
           << "[" << level << "]" 
           << "[" << information.m_user << "]"
           << " "
           << information.m_stream.str();
    return stream.str();
}

// ----------------------------------------------------------------------------

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

FileAppender::FileAppender(std::string filename, 
                           std::shared_ptr<Layout> layout)
        : Appender(std::move(layout)), 
          m_filename(std::move(filename))
{
    m_ofstream.open(m_filename, std::ofstream::out | std::ofstream::app);
    // 如果无法打开目标文件
    if (!m_ofstream.is_open())
        throw std::logic_error("No such file: " + m_filename); 
}

FileAppender::~FileAppender()
{
    m_ofstream.close();
}

void FileAppender::write(Level level, const Event::Information& information)
{
    m_ofstream << m_layout->formatter(level, information) << std::endl;
}

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

std::shared_ptr<LoggerManager> LoggerManager::instance(new LoggerManager());

std::shared_ptr<LoggerManager> LoggerManager::Instance() 
{
    return instance;    
}

LoggerManager::LoggerManager()
{
    // 默认有一个系统用户，日志级别为 INFO
    m_table["system"] = std::make_shared<Logger>(Level::INFO);    
    // 给系统用户添加一个默认的控制台输出地
    m_table["system"]->add_appender(
        std::make_shared<ConsoleAppender>(
            std::make_shared<Layout>()
        )
    );

    // 默认的注册登录行为
    m_register_behavior = 0;
    m_login_behavior = 0;

    // 行为列表
    m_register_behavior_list.emplace_back(
        std::bind(&LoggerManager::register_behavior_0, this, 
            std::placeholders::_1, std::placeholders::_2)
    );
    m_register_behavior_list.emplace_back(
        std::bind(&LoggerManager::register_behavior_1, this, 
            std::placeholders::_1, std::placeholders::_2)
    );
    m_register_behavior_list.emplace_back(
        std::bind(&LoggerManager::register_behavior_2, this, 
            std::placeholders::_1, std::placeholders::_2)
    );

    m_login_behavior_list.emplace_back(
        std::bind(&LoggerManager::login_behavior_0, this,
            std::placeholders::_1)
    );
    m_login_behavior_list.emplace_back(
        std::bind(&LoggerManager::login_behavior_1, this,
            std::placeholders::_1)
    );
    m_login_behavior_list.emplace_back(
        std::bind(&LoggerManager::login_behavior_2, this,
            std::placeholders::_1)
    );
    m_login_behavior_list.emplace_back(
        std::bind(&LoggerManager::login_behavior_3, this,
            std::placeholders::_1)
    );
}

std::shared_ptr<Logger> LoggerManager::doRegister(const std::string& user, 
                                                  Level level)
{
    // 用户名一概小写
    std::string lower_user = o7si::utils::to_lower(user);

    // 如果行为未定义，则使用默认行为
    if (m_register_behavior >= m_register_behavior_list.size())
        m_register_behavior = 0;

    return m_register_behavior_list[m_register_behavior](lower_user, level);
}

std::shared_ptr<Logger> LoggerManager::doLogin(const std::string& user)
{
    /// 用户名一概小写
    std::string lower_user = o7si::utils::to_lower(user);
    
    // 如果行为未定义，则使用默认行为
    if (m_login_behavior >= m_login_behavior_list.size())
        m_login_behavior = 0;

    return m_login_behavior_list[m_login_behavior](lower_user);
}

std::shared_ptr<Logger> 
LoggerManager::register_behavior_0(const std::string& user, Level level)
{
    // 用户未被注册，正常注册并返回对应指针
    if (m_table.find(user) == m_table.end())
    {
        m_table[user] = std::make_shared<Logger>(level);    
        return m_table[user];
    }
    // 用户已被注册，返回对应指针
    return m_table[user];
}

std::shared_ptr<Logger> 
LoggerManager::register_behavior_1(const std::string& user, Level level)
{
    // 用户未被注册，正常注册并返回对应指针
    if (m_table.find(user) == m_table.end())
    {
        m_table[user] = std::make_shared<Logger>(level);    
        return m_table[user];
    }
    // 用户已被注册，返回空指针
    return nullptr;
}

std::shared_ptr<Logger> 
LoggerManager::register_behavior_2(const std::string& user, Level level)
{
    // 禁止注册，返回空指针
    return nullptr;
}

std::shared_ptr<Logger> 
LoggerManager::login_behavior_0(const std::string& user)
{
    // 用户已被注册，返回对应指针
    if (m_table.find(user) != m_table.end())
        return m_table[user];
    // 用户未被注册，帮助其完成注册，返回对应指针
    m_table[user] = std::make_shared<Logger>(Level::DEBUG);
    return m_table[user];
}

std::shared_ptr<Logger> 
LoggerManager::login_behavior_1(const std::string& user)
{
    // 用户已被注册，返回对应指针
    if (m_table.find(user) != m_table.end())
        return m_table[user];
    // 用户未被注册，帮助其完成注册，并添加一个默认的控制台输出地，返回对应指针
    m_table[user] = std::make_shared<Logger>(Level::DEBUG);
    m_table[user]->add_appender(
        std::make_shared<ConsoleAppender>(
            std::make_shared<Layout>()
        )
    );
    return m_table[user];
}

std::shared_ptr<Logger>
 LoggerManager::login_behavior_2(const std::string& user)
{
    // 用户已被注册，返回对应指针
    if (m_table.find(user) != m_table.end())
        return m_table[user];
    // 用户未被注册，返回空指针
    return nullptr;
}

std::shared_ptr<Logger>
 LoggerManager::login_behavior_3(const std::string& user)
{
    // 禁止登录，返回空指针
    return nullptr;
}

// ----------------------------------------------------------------------------

}   // namespace log
}   // namespace o7si
