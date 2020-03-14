/******************************************************************************
 * File: log.h
 * Description: 记录日志输出。
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils.h"


// 日志的系统用户，用于框架的内部输出
#define LOG_SYSTEM_USER "system"

// 为实用性和灵活性考虑，定义了多组日志宏
// 宏定义中出现 DEBUG/INFO/WARN/ERROR/FATAL 表示日志的输出级别
// 宏定义中出现 CHECK 时，表示输出日志时会进行条件判断
// 宏定义中出现 SYS 时，表示以系统身份进行日志输出
#define LOG_TEMPLATE(level, user, condition) \
    o7si::log::Event( \
            o7si::log::LoggerManager::Instance()->doLogin(#user), \
            o7si::log::GenLevelFrom(#level), \
            o7si::log::Event::Information( \
                o7si::utils::get_timestamp(), \
                o7si::utils::get_thread_id(), \
                o7si::utils::get_thread_name(), \
                __FILE__, \
                __FUNCTION__, \
                __LINE__, \
                o7si::utils::to_lower(#user) \
            ), \
            condition \
        ).stream()

// 普通的日志宏
#define LOG(level, user) \
        LOG_TEMPLATE(level, user, true)

#define LOG_DEBUG(user) \
        LOG(DEBUG, user)
#define LOG_INFO(user) \
        LOG(INFO, user)
#define LOG_WARN(user) \
        LOG(WARN, user)
#define LOG_ERROR(user) \
        LOG(ERROR, user)
#define LOG_FATAL(user) \
        LOG(FATAL, user)

#define LOG_DEBUG_SYS \
        LOG_DEBUG(SYSTEM)
#define LOG_INFO_SYS \
        LOG_INFO(SYSTEM)
#define LOG_WARN_SYS \
        LOG_WARN(SYSTEM)
#define LOG_ERROR_SYS \
        LOG_ERROR(SYSTEM)
#define LOG_FATAL_SYS \
        LOG_FATAL(SYSTEM)


// 带检查的日志宏
#define LOG_CHECK(level, user, check) \
        LOG_TEMPLATE(level, user, check)

#define LOG_CHECK_SYS(level, check) \
        LOG_CHECK(level, SYSTEM, check)

#define LOG_DEBUG_CHECK(user, check) \
        LOG_CHECK(DEBUG, user, check)
#define LOG_INFO_CHECK(user, check) \
        LOG_CHECK(INFO, user, check)
#define LOG_WARN_CHECK(user, check) \
        LOG_CHECK(WARN, user, check)
#define LOG_ERROR_CHECK(user, check) \
        LOG_CHECK(ERROR, user, check)
#define LOG_FATAL_CHECK(user, check) \
        LOG_CHECK(FATAL, user, check)

#define LOG_DEBUG_CHECK_SYS(check) \
        LOG_DEBUG_CHECK(SYSTEM, check)
#define LOG_INFO_CHECK_SYS(check)  \
        LOG_INFO_CHECK(SYSTEM, check)
#define LOG_WARN_CHECK_SYS(check)  \
        LOG_WARN_CHECK(SYSTEM, check)
#define LOG_ERROR_CHECK_SYS(check)  \
        LOG_ERROR_CHECK(SYSTEM, check)
#define LOG_FATAL_CHECK_SYS(check)  \
        LOG_FATAL_CHECK(SYSTEM, check)


namespace o7si
{
namespace log
{

// ----------------------------------------------------------------------------

// 日志级别枚举类，共提供 8 种日志级别
// 1. UNKNOWN   ：未知类型
// 2. ALL       ：打开所有日志记录
// 3. DEBUG     ：调试信息
// 4. INFO      ：运行信息
// 5. WARN      ：警告信息
// 6. ERROR     ：错误信息
// 7. FATAL     ：严重错误信息
// 8. OFF       ：关闭所有日志记录
enum class Level
{
    UNKNOWN = 0,
    ALL,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    OFF
};

// 可以通过字符串或者数字（不推荐）获取对应的日志级别对象
// 当没有对应级别时，使用 UNKNOWN 表示
Level GenLevelFrom(const std::string& str);
Level GenLevelFrom(size_t num);

// 每一个日志级别都有对应的字符串进行表示
std::ostream& operator<<(std::ostream& stream, const Level& level);

// ----------------------------------------------------------------------------

class Logger;

// 日志事件
// 存储日志信息，析构时将日志输出至输出地
class Event
{
public:
    // 日志事件信息
    struct Information
    {
        Information(time_t time,
                    uint32_t threadId, std::string threadName,
                    std::string fileName, std::string funcName, uint32_t line,
                    std::string user);

        // 时间戳
        time_t m_time;
        // 线程 ID
        uint32_t m_threadId;
        // 线程名称
        std::string m_threadName;
        // 文件名称
        std::string m_fileName;
        // 函数名称
        std::string m_funcName;
        // 行号
        uint32_t m_line;
        // 用户
        std::string m_user;
        // 内容
        std::ostringstream m_stream;
    };

    Event(std::shared_ptr<Logger> logger, 
          Level level, Information information, 
          bool condition = true);

    // 析构函数会被自动调用，在此处完成日志的记录工作
    ~Event();

    std::ostringstream& stream()
    {
        return m_information.m_stream;
    }

private:
    // 日志事件信息
    Information m_information;
    // 条件检查
    bool m_condition;
    // 日志级别
    Level m_level;
    // 日志器
    std::shared_ptr<Logger> m_logger;
};

// ----------------------------------------------------------------------------

// 日志格式控制器
// 调整日志的输出格式
// 由于格式化时需要消耗一定的资源，建议使用默认格式
// 即"[%d][%i:%t][%f:%m:%l][%v][%u] %c"
class Layout
{
public:
    explicit Layout(std::string pattern = "default");
    
    std::string pattern() const
    {
        return m_pattern;
    }

    void pattern(std::string pattern)
    {
        m_pattern = std::move(pattern);
    }

    std::string formatter(Level level, const Event::Information& information);

private:
    // 格式约定
    // %d   日期时间
    // %i   线程 ID
    // %t   线程名称
    // %f   文件名称
    // %m   函数名称
    // %l   行号
    // %v   日志级别
    // %u   用户
    // %c   内容
    std::string m_pattern;
};

// ----------------------------------------------------------------------------

// 日志输出地
class Appender
{
public:
    explicit Appender(std::shared_ptr<Layout> layout);

    virtual ~Appender() = default;
    
    std::shared_ptr<Layout> layout()
    {
        return m_layout;    
    } 

    void layout(std::shared_ptr<Layout> layout)
    {
        m_layout = layout;
    }

    virtual void write(Level level, const Event::Information& information) = 0;

protected:
    // 日志格式化
    std::shared_ptr<Layout> m_layout;
};

// 日志输出地：控制台
// 将日志输出至标准输出
class ConsoleAppender : public Appender
{
public:
    explicit ConsoleAppender(std::shared_ptr<Layout> layout);
    
    void write(Level level, const Event::Information& information) override;
};

// 日志输出地：文件
// 将日志输出至目标文件，当文件无法打开时将抛出异常
class FileAppender : public Appender
{
public:
    explicit FileAppender(std::string filename, std::shared_ptr<Layout> layout);

    ~FileAppender();

    std::string filename() const
    {
        return m_filename; 
    }
    
    void filename(std::string filename)
    {
        m_filename = std::move(filename);    
    }

    void write(Level level, const Event::Information& information) override;

private:
    // 文件名称
    std::string m_filename;
    // 文件流
    std::ofstream m_ofstream;
};

// ----------------------------------------------------------------------------

// 日志器
class Logger
{
public:
    explicit Logger(Level baseline = Level::DEBUG);

    ~Logger();

    Level level() const
    {
        return m_baseline;    
    }
    
    void level(Level value)
    {
        m_baseline = value;
    }

    void appenders(std::initializer_list<std::shared_ptr<Appender>> appenders)
    {
        m_appenders = appenders;
    }

    std::list<std::shared_ptr<Appender>> appenders()
    {
        return m_appenders;
    }

    void add_appender(std::shared_ptr<Appender> appender)
    {
        m_appenders.push_front(appender);
    }

    void clear_appenders()
    {
        m_appenders.clear();    
    }

    void log(Level level, const Event::Information& information);

private:
    // 日志级别，默认认 DEBUG 级别
    Level m_baseline;
    // 日志输出地列表
    std::list<std::shared_ptr<Appender>> m_appenders;
};

// ----------------------------------------------------------------------------

// 日志器的控制器（单例模式）
// 提供日志器的注册和登录功能
// 可修改注册/登录时的行为
class LoggerManager
{
public:
    // 注册，供多个用户进行使用
    std::shared_ptr<Logger> doRegister(const std::string& user, Level level); 

    // 登录
    std::shared_ptr<Logger> doLogin(const std::string& user);
    
    int register_behavior() const
    {
        return m_register_behavior;    
    }

    void register_behavior(int behavior)
    {
        m_register_behavior = behavior;    
    }

    int login_behavior() const
    {
        return m_login_behavior;    
    }

    void login_behavior(int behavior)
    {
        m_login_behavior = behavior;    
    } 

public:
    // 获取 LoggerManager 的单例对象
    static std::shared_ptr<LoggerManager> Instance();

    // 删除拷贝构造函数，防止用户拷贝对象
    LoggerManager(const LoggerManager& other) = delete;

    // 删除移动构造函数，防止用户使用移动语义
    LoggerManager(LoggerManager&& other) = delete;

private:
    // 注册行为：0
    //      用户未被注册，正常注册并返回对应指针
    //      用户已被注册，返回对应指针
    std::shared_ptr<Logger> register_behavior_0(const std::string& user, 
                                                Level level);

    // 注册行为：1
    //      用户未被注册，正常注册并返回对应指针
    //      用户已被注册，返回空指针
    std::shared_ptr<Logger> register_behavior_1(const std::string& user, 
                                                Level level);

    // 注册行为：2
    //      禁止注册，返回空指针
    std::shared_ptr<Logger> register_behavior_2(const std::string& user, 
                                                Level level);

    // 登录行为：0
    //      用户已被注册，返回对应指针
    //      用户未被注册，帮助其完成注册，返回对应指针
    std::shared_ptr<Logger> login_behavior_0(const std::string& user);

    // 登录行为：1
    //      用户已被注册，返回对应指针
    //      用户未被注册，帮助其完成注册并添加一个控制台输出地，返回对应指针
    std::shared_ptr<Logger> login_behavior_1(const std::string& user);

    // 登录行为：2
    //      用户已被注册，返回对应指针
    //      用户未被注册，返回空指针
    std::shared_ptr<Logger> login_behavior_2(const std::string& user);

    // 登录行为：3
    //      禁止登录，返回空指针
    std::shared_ptr<Logger> login_behavior_3(const std::string& user);

private: 
    /// 将构造函数修饰为私有，防止用户擅自实例化对象
    LoggerManager();   

    // 注册行为
    int m_register_behavior;
    using RegisterBehavior 
        = std::function<std::shared_ptr<Logger>(const std::string&, Level)>;
    std::vector<RegisterBehavior> m_register_behavior_list;

    /// 登录行为
    int m_login_behavior;
    using LoginBehavior 
        = std::function<std::shared_ptr<Logger>(const std::string&)>;
    std::vector<LoginBehavior> m_login_behavior_list;

    // 映射表
    std::unordered_map<std::string, std::shared_ptr<Logger>> m_table;
    // 单例对象
    static std::shared_ptr<LoggerManager> instance;
};

// ----------------------------------------------------------------------------

}   // namespace log
}   // namespace o7si
