#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <list>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <functional>

#include "utils.h"

/// 日志的宏定义模板
#define LOG_TEMPLATE(level, user, check) \
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
                o7si::utils::to_lower(#user)), \
            check \
        ).stream()

/// 普通日志
#define LOG(level, user) LOG_TEMPLATE(level, user, true)

#define LOG_DEBUG(user) LOG(DEBUG, user)
#define LOG_INFO(user) LOG(INFO, user)
#define LOG_WARN(user) LOG(WARN, user)
#define LOG_ERROR(user) LOG(ERROR, user)
#define LOG_FATAL(user) LOG(FATAL, user)

#define LOG_DEBUG_SYS LOG_DEBUG(SYSTEM)
#define LOG_INFO_SYS LOG_INFO(SYSTEM)
#define LOG_WARN_SYS LOG_WARN(SYSTEM)
#define LOG_ERROR_SYS LOG_ERROR(SYSTEM)
#define LOG_FATAL_SYS LOG_FATAL(SYSTEM)

/// 带检查的日志，当 check 与 target 的值相同时才输出日志
#define LOG_CHECK(level, user, check, target) LOG_TEMPLATE(level, user, (check) == target)

#define LOG_CHECK_SYS(level, check, target) LOG_CHECK(level, SYSTEM, check, target)

#define LOG_DEBUG_CHECK(user, check, target) LOG_CHECK(DEBUG, user, check, target)
#define LOG_INFO_CHECK(user, check, target) LOG_CHECK(INFO, user, check, target)
#define LOG_WARN_CHECK(user, check, target) LOG_CHECK(WARN, user, check, target)
#define LOG_ERROR_CHECK(user, check, target) LOG_CHECK(ERROR, user, check, target)
#define LOG_FATAL_CHECK(user, check, target) LOG_CHECK(FATAL, user, check, target)

#define LOG_DEBUG_CHECK_SYS(check, target) LOG_DEBUG_CHECK(SYSTEM, check, target)
#define LOG_INFO_CHECK_SYS(check, target) LOG_INFO_CHECK(SYSTEM, check, target)
#define LOG_WARN_CHECK_SYS(check, target) LOG_WARN_CHECK(SYSTEM, check, target)
#define LOG_ERROR_CHECK_SYS(check, target) LOG_ERROR_CHECK(SYSTEM, check, target)
#define LOG_FATAL_CHECK_SYS(check, target) LOG_FATAL_CHECK(SYSTEM, check, target)

#define LOG_SYS_CHECK LOG_CHECK_SYS

#define LOG_DEBUG_SYS_CHECK LOG_DEBUG_CHECK_SYS
#define LOG_INFO_SYS_CHECK LOG_INFO_CHECK_SYS
#define LOG_WARN_SYS_CHECK LOG_WARN_CHECK_SYS
#define LOG_ERROR_SYS_CHECK LOG_ERROR_CHECK_SYS
#define LOG_FATAL_SYS_CHECK LOG_FATAL_CHECK_SYS

/// 带检查的日志，当 check 值为真时才输出日志
#define LOG_CHECK_TRUE(level, user, check) LOG_TEMPLATE(level, user, (check) == true)

#define LOG_CHECK_TRUE_SYS(level, check) LOG_CHECK_TRUE(level, SYSTEM, check)

#define LOG_DEBUG_CHECK_TRUE(user, check) LOG_CHECK_TRUE(DEBUG, user, check)
#define LOG_INFO_CHECK_TRUE(user, check) LOG_CHECK_TRUE(INFO, user, check)
#define LOG_WARN_CHECK_TRUE(user, check) LOG_CHECK_TRUE(WARN, user, check)
#define LOG_ERROR_CHECK_TRUE(user, check) LOG_CHECK_TRUE(ERROR, user, check)
#define LOG_FATAL_CHECK_TRUE(user, check) LOG_CHECK_TRUE(FATAL, user, check)

#define LOG_DEBUG_CHECK_TRUE_SYS(check) LOG_DEBUG_CHECK_TRUE(SYSTEM, check)
#define LOG_INFO_CHECK_TRUE_SYS(check) LOG_INFO_CHECK_TRUE(SYSTEM, check)
#define LOG_WARN_CHECK_TRUE_SYS(check) LOG_WARN_CHECK_TRUE(SYSTEM, check)
#define LOG_ERROR_CHECK_TRUE_SYS(check) LOG_ERROR_CHECK_TRUE(SYSTEM, check)
#define LOG_FATAL_CHECK_TRUE_SYS(check) LOG_FATAL_CHECK_TRUE(SYSTEM, check)

#define LOG_SYS_CHECK_TRUE LOG_CHECK_TRUE_SYS

#define LOG_DEBUG_SYS_CHECK_TRUE LOG_DEBUG_CHECK_TRUE_SYS
#define LOG_INFO_SYS_CHECK_TRUE LOG_INFO_CHECK_TRUE_SYS
#define LOG_WARN_SYS_CHECK_TRUE LOG_WARN_CHECK_TRUE_SYS
#define LOG_ERROR_SYS_CHECK_TRUE LOG_ERROR_CHECK_TRUE_SYS
#define LOG_FATAL_SYS_CHECK_TRUE LOG_FATAL_CHECK_TRUE_SYS

/// 带检查的日志，当 check 值为假时才输出日志
#define LOG_CHECK_FALSE(level, user, check) LOG_TEMPLATE(level, user, (check) == false)

#define LOG_CHECK_FALSE_SYS(level, check) LOG_CHECK_FALSE(level, SYSTEM, check)

#define LOG_DEBUG_CHECK_FALSE(user, check) LOG_CHECK_FALSE(DEBUG, user, check)
#define LOG_INFO_CHECK_FALSE(user, check) LOG_CHECK_FALSE(INFO, user, check)
#define LOG_WARN_CHECK_FALSE(user, check) LOG_CHECK_FALSE(WARN, user, check)
#define LOG_ERROR_CHECK_FALSE(user, check) LOG_CHECK_FALSE(ERROR, user, check)
#define LOG_FATAL_CHECK_FALSE(user, check) LOG_CHECK_FALSE(FATAL, user, check)

#define LOG_DEBUG_CHECK_FALSE_SYS(check) LOG_DEBUG_CHECK_FALSE(SYSTEM, check)
#define LOG_INFO_CHECK_FALSE_SYS(check) LOG_INFO_CHECK_FALSE(SYSTEM, check)
#define LOG_WARN_CHECK_FALSE_SYS(check) LOG_WARN_CHECK_FALSE(SYSTEM, check)
#define LOG_ERROR_CHECK_FALSE_SYS(check) LOG_ERROR_CHECK_FALSE(SYSTEM, check)
#define LOG_FATAL_CHECK_FALSE_SYS(check) LOG_FATAL_CHECK_FALSE(SYSTEM, check)

#define LOG_SYS_CHECK_FALSE LOG_CHECK_FALSE_SYS

#define LOG_DEBUG_SYS_CHECK_FALSE LOG_DEBUG_CHECK_FALSE_SYS
#define LOG_INFO_SYS_CHECK_FALSE LOG_INFO_CHECK_FALSE_SYS
#define LOG_WARN_SYS_CHECK_FALSE LOG_WARN_CHECK_FALSE_SYS
#define LOG_ERROR_SYS_CHECK_FALSE LOG_ERROR_CHECK_FALSE_SYS
#define LOG_FATAL_SYS_CHECK_FALSE LOG_FATAL_CHECK_FALSE_SYS

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
    Event(std::shared_ptr<Logger> logger, Level level, Information information, bool condition = true);

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
    /// 条件检查
    bool m_condition;
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
    
    /// 注册时的行为定义
    void register_behavior(int behavior)
    {
        m_register_behavior = behavior;    
    }

    /// 登录时的行为定义
    void login_behavior(int behavior)
    {
        m_login_behavior = behavior;    
    } 

public:
    /// 获取 LoggerManager 的单例对象
    static std::shared_ptr<LoggerManager> Instance();

    /// 删除拷贝构造函数，防止用户拷贝对象
    LoggerManager(const LoggerManager& other) = delete;

    /// 删除移动构造函数，防止用户使用移动语义
    LoggerManager(LoggerManager&& other) = delete;

private:
    /// 注册行为：0
    std::shared_ptr<Logger> register_behavior_0(const std::string& user, Level level);

    /// 注册行为：1
    std::shared_ptr<Logger> register_behavior_1(const std::string& user, Level level);

    /// 注册行为：2
    std::shared_ptr<Logger> register_behavior_2(const std::string& user, Level level);

    /// 登录行为：0
    std::shared_ptr<Logger> login_behavior_0(const std::string& user);

    /// 登录行为：1
    std::shared_ptr<Logger> login_behavior_1(const std::string& user);

    /// 登录行为：2
    std::shared_ptr<Logger> login_behavior_2(const std::string& user);

    /// 登录行为：3
    std::shared_ptr<Logger> login_behavior_3(const std::string& user);

private: 
    /// 将构造函数修饰为私有，防止用户擅自实例化对象
    LoggerManager();   

    /// 注册行为
    /// 0：
    ///     用户未被注册，正常注册并返回对应指针
    ///     用户已被注册，返回对应指针
    /// 1：
    ///     用户未被注册，正常注册并返回对应指针
    ///     用户已被注册，返回空指针
    /// 2：
    ///     禁止注册，返回空指针
    int m_register_behavior;
    using RegisterBehavior = std::function<std::shared_ptr<Logger>(const std::string&, Level)>;
    std::vector<RegisterBehavior> m_register_behavior_list;

    /// 登录行为
    /// 0：
    ///     用户已被注册，返回对应指针
    ///     用户未被注册，帮助其完成注册，返回对应指针
    /// 1：
    ///     用户已被注册，返回对应指针
    ///     用户未被注册，帮助其完成注册，并添加一个默认的控制台输出地，返回对应指针
    /// 2：
    ///     用户已被注册，返回对应指针
    ///     用户未被注册，返回空指针
    /// 3：
    ///     禁止登录，返回空指针
    int m_login_behavior;
    using LoginBehavior = std::function<std::shared_ptr<Logger>(const std::string&)>;
    std::vector<LoginBehavior> m_login_behavior_list;

    /// 映射表
    std::unordered_map<std::string, std::shared_ptr<Logger>> m_table;
    /// 单例对象
    static std::shared_ptr<LoggerManager> instance;
};

// ---------------------------------------------------------------------------------------------------------------------

}   // namespace log
}   // namespace o7si
