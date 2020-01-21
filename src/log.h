#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <list>
#include <chrono>
#include <thread>

#include "utils.h"

#define LOG(level) \
    o7si::log::Event( \
            o7si::log::Logger::getInstance(), \
            o7si::log::FromString(#level), \
            o7si::log::Event::Information( \
                std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()), \
                o7si::utils::convertThreadId(std::this_thread::get_id()), \
                "()", \
                __FILE__, \
                __FUNCTION__, \
                __LINE__ \
    )).stream()

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

/// 获取日志级别对应的文本
const char* ToString(Level level);

/// 通过文本获取日志级别对象
Level FromString(const std::string& level);

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
        /// 内容
        std::ostringstream m_stream;

        std::ostringstream& stream()
        {
            return m_stream;
        }

        Information(time_t time,
                    uint32_t threadId, std::string threadName,
                    std::string fileName, std::string funcName, uint32_t line);
    };

    Event(std::shared_ptr<Logger> logger, Level level, Information information);

    ~Event();

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
    explicit Layout(std::string pattern);
    std::string formatter(Level level, const Event::Information& information);

private:
    std::string m_pattern;
};

// ---------------------------------------------------------------------------------------------------------------------

/// 日志输出地
class Appender
{
public:
    explicit Appender(std::shared_ptr<Layout> layout);
    virtual ~Appender() = default;

    virtual void write(Level level, const Event::Information& information) = 0;

protected:
    std::shared_ptr<Layout> m_layout;
};

/// 日志输出地：控制台
class ConsoleAppender : public Appender
{
public:
    explicit ConsoleAppender(std::shared_ptr<Layout> layout);
    void write(Level level, const Event::Information& information) override;
};

/// 日志输出地：文件
class FileAppender : public Appender
{
public:
    explicit FileAppender(std::string fileName, std::shared_ptr<Layout> layout);

    ~FileAppender() override;

    void write(Level level, const Event::Information& information) override;

private:
    std::string m_fileName;
    std::ofstream m_ofstream;
};

// ---------------------------------------------------------------------------------------------------------------------

/// 日志
class Logger
{
public:
    void log(Level level, const Event::Information& information);

    std::shared_ptr<Logger> setLevel(Level level)
    {
        baseline = level;
        return instance;
    }

    std::shared_ptr<Logger> setAppenders(std::initializer_list<std::shared_ptr<Appender>> appenders)
    {
        m_appenders = appenders;
        return instance;
    }

public:
    /// 获取 Logger 的单例对象
    static std::shared_ptr<Logger> getInstance();

    /// 删除拷贝构造函数，防止用户拷贝该对象
    Logger(const Logger& other) = delete;

private:
    /// 将构造函数修饰为私有，防止用户擅自实例化对象
    explicit Logger(Level level = Level::DEBUG);

    /// 日志级别，当大于该级别才进行输出，默认为 DEBUG
    Level baseline;
    /// 日志输出地列表
    std::list<std::shared_ptr<Appender>> m_appenders;
    /// 单例对象
    static std::shared_ptr<Logger> instance;
};

// ---------------------------------------------------------------------------------------------------------------------

}   // namespace log
}   // namespace o7si