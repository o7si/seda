#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <list>

namespace o7si
{
namespace log
{

// ---------------------------------------------------------------------------------------------------------------------

/// 日志级别枚举类
enum class Level
{
    UNKNOWN,
    ALL,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    OFF
};

/// 获取日志级别对应的文本
const char* ToString(Level level);

/// 通过文本获取日志级别对象
Level FromString(const std::string& level);

// ---------------------------------------------------------------------------------------------------------------------


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
        uint64_t m_time;
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

        Information(uint64_t time,
                    uint32_t threadId, std::string threadName,
                    std::string fileName, std::string funcName, uint32_t line);
    };

    Event(std::shared_ptr<Logger> logger, Level level, Information  information);

    ~Event();
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
    std::string formatter(Level level, const Event::Information& information);
};

// ---------------------------------------------------------------------------------------------------------------------

/// 日志输出地
class Appender
{
public:
    virtual void write(Level level, const Event::Information& information) = 0;
    virtual ~Appender() = default;

protected:
    std::shared_ptr<Layout> m_layout;
};

/// 日志输出地：控制台
class ConsoleAppender : public Appender
{
public:
    void write(Level level, const Event::Information& information) override;
};

/// 日志输出地：文件
class FileAppender : public Appender
{
public:
    FileAppender(std::string fileName);
    ~FileAppender() override;
    void write(Level level, const Event::Information& information) override;

private:
    std::string m_fileName;
    std::ofstream m_ofstream;
};

// ---------------------------------------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------

/// 日志
class Logger
{
public:
    void log(Level level, const Event::Information& information);
    void setAppenders(std::initializer_list<std::shared_ptr<Appender>> appenders)
    {
        m_appenders = appenders;
    }
public:
    /// 获取 Logger 的单例对象
    static std::shared_ptr<Logger> getInstance();

    /// 删除拷贝构造函数，防止用户拷贝该对象
    Logger(const Logger& other) = delete;

private:
    /// 将构造函数修饰为私有，防止用户擅自实例化对象
    Logger() = default;

    /// 日志输出地列表
    std::list<std::shared_ptr<Appender>> m_appenders;
    /// 单例对象
    static std::shared_ptr<Logger> instance;
};

// ---------------------------------------------------------------------------------------------------------------------

}   // namespace log
}   // namespace o7si