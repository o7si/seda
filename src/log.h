#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <list>
#include <unordered_map>
#include <memory>
#include <chrono>

#include "utils.h"

#define LOG(level, user) \
    o7si::log::Event( \
            o7si::log::LoggerManager::Instance()->doLogin(#user), \
            o7si::log::GenLevelFrom(#level), \
            o7si::log::Event::Information( \
                std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), \
                o7si::utils::get_thread_id(), \
                o7si::utils::get_thread_name(), \
                __FILE__, \
                __FUNCTION__, \
                __LINE__, \
                o7si::utils::to_lower(#user) \
    )).stream()

/// 用户输出
#define LOG_DEBUG(user) LOG(DEBUG, user)
#define LOG_INFO(user) LOG(INFO, user)
#define LOG_WARN(user) LOG(WARN, user)
#define LOG_ERROR(user) LOG(ERROR, user)
#define LOG_FATAL(user) LOG(TATAL, user)

/// 系统输出
#define LOG_DEBUG_SYS LOG(DEBUG, SYS)
#define LOG_INFO_SYS LOG(INFO, SYS)
#define LOG_WARN_SYS LOG(WARN, SYS)
#define LOG_ERROR_SYS LOG(ERROR, SYS)
#define LOG_FATAL_SYS LOG(FATAL, SYS)

namespace o7si
{
namespace log
{

// ---------------------------------------------------------------------------------------------------------------------

/// 日志级别枚举类
enum class Level
{
    /// 未知类型
    UNKNOWN = 0,
    /// 打开所有日志记录
    ALL,
    /// 调试信息
    DEBUG,
    /// 运行信息
    INFO,
    /// 警告信息
    WARN,
    /// 错误信息
    ERROR,
    /// 严重错误信息
    FATAL,
    /// 关闭所有日志记录
    OFF
};

/// 通过文本获取日志级别对象
Level GenLevelFrom(const std::string& level);

/// 以字符串的形式输出日志级别对象
std::ostream& operator<<(std::ostream& stream, const Level& level);

// ---------------------------------------------------------------------------------------------------------------------

class Logger;

/// 日志事件
class Event
{
public:
    /// 日志事件信息
    struct Information
    {
        /// 时间戳
        time_t m_time;
        /// 线程 ID
        uint32_t m_threadId;
        /// 线程名称
        std::string m_threadName;
        /// 文件名称
        std::string m_fileName;
        /// 函数名称
        std::string m_funcName;
        /// 行号
        uint32_t m_line;
        /// 用户
        std::string m_user;
        /// 内容
        std::ostringstream m_stream;

        /// 获取字符串流
        std::ostringstream& stream()
        {
            return m_stream;
        }

        /// 构造函数
        Information(time_t time,
                    uint32_t threadId, std::string threadName,
                    std::string fileName, std::string funcName, uint32_t line,
                    std::string user);
    };

    /// 构造函数
    Event(std::shared_ptr<Logger> logger, Level level, Information information);

    /// 析构函数
    ~Event();

    /// 获取字符串流
    std::ostringstream& stream()
    {
        return m_information.stream();
    }

private:
    /// 日志事件信息
    Information m_information;
    /// 日志级别
    Level m_level;
    /// 日志器
    std::shared_ptr<Logger> m_logger;
};

// ---------------------------------------------------------------------------------------------------------------------

/// 日志格式控制器
class Layout
{
public:
    /// 构造函数
    explicit Layout(std::string pattern = "");
    
    /// 格式化日志
    std::string formatter(Level level, const Event::Information& information);

private:
    /// 日志格式
    std::string m_pattern;
};

// ---------------------------------------------------------------------------------------------------------------------

/// 日志输出地
class Appender
{
public:
    /// 构造函数
    explicit Appender(std::shared_ptr<Layout> layout);

    /// 析构函数
    virtual ~Appender() = default;

    /// 记录日志
    virtual void write(Level level, const Event::Information& information) = 0;

protected:
    /// 日志格式化
    std::shared_ptr<Layout> m_layout;
};

/// 日志输出地：控制台
class ConsoleAppender : public Appender
{
public:
    /// 构造函数
    explicit ConsoleAppender(std::shared_ptr<Layout> layout);
    
    /// 记录日志
    void write(Level level, const Event::Information& information) override;
};

/// 日志输出地：文件
class FileAppender : public Appender
{
public:
    /// 构造函数
    explicit FileAppender(std::string fileName, std::shared_ptr<Layout> layout);

    /// 析构函数
    ~FileAppender();

    /// 记录日志
    void write(Level level, const Event::Information& information) override;

private:
    /// 文件名称
    std::string m_filename;
    /// 文件流
    std::ofstream m_ofstream;
};

// ---------------------------------------------------------------------------------------------------------------------

/// 日志
class Logger
{
public:
    /// 构造函数
    explicit Logger(Level baseline);

    /// 析构函数
    ~Logger();

    /// 设置日志级别
    void level(Level value)
    {
        m_baseline = value;
    }

    /// 获取日志级别
    Level level() const
    {
        return m_baseline;    
    }

    /// 设置日志输出地列表
    void appenders(std::initializer_list<std::shared_ptr<Appender>> appenders)
    {
        m_appenders = appenders;
    }

    /// 获取日志输出地列表
    std::list<std::shared_ptr<Appender>> appenders() const
    {
        return m_appenders;
    }

    /// 添加一个日志输出地
    void add_appender(std::shared_ptr<Appender> appender)
    {
        m_appenders.push_front(appender);
    }

    /// 清空日志输出地
    void clear_appenders()
    {
        m_appenders.clear();    
    }

    /// 记录日志 
    void log(Level level, const Event::Information& information);

private:
    /// 日志级别，当大于该级别才进行输出，默认为 DEBUG
    Level m_baseline;
    /// 日志输出地列表
    std::list<std::shared_ptr<Appender>> m_appenders;
};

// ---------------------------------------------------------------------------------------------------------------------

class LoggerManager
{
public:
    /// 注册，供多个用户进行使用
    std::shared_ptr<Logger> doRegister(const std::string& user, Level level); 

    /// 登录
    std::shared_ptr<Logger> doLogin(const std::string& user);

public:
    /// 获取 LoggerManager 的单例对象
    static std::shared_ptr<LoggerManager> Instance();

    /// 删除拷贝构造函数，防止用户拷贝对象
    LoggerManager(const LoggerManager& other) = delete;

    /// 删除移动构造函数，防止用户使用移动语义
    LoggerManager(LoggerManager&& other) = delete;

private: 
    /// 将构造函数修饰为私有，防止用户擅自实例化对象
    LoggerManager();   

    /// 映射表
    std::unordered_map<std::string, std::shared_ptr<Logger>> m_table;
    /// 单例对象
    static std::shared_ptr<LoggerManager> instance;
};

// ---------------------------------------------------------------------------------------------------------------------

}   // namespace log
}   // namespace o7si
