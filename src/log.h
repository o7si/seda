#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <list>

namespace o7si
{
namespace log
{

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


/// 日志格式控制器
class Layout
{

};


/// 日志输出地
class Appender
{

};

/// 日志
class Logger
{
public:
    /// 日志信息
    struct Information
    {
        /// 日志级别
        Level m_level;
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

        Information();

        Information(Level level, uint64_t time,
                    uint32_t threadId, std::string threadName,
                    std::string fileName, std::string funcName, uint32_t line);
    };
public:
    /// 获取 Logger 的单例对象
    static std::shared_ptr<Logger> getInstance();

    /// 删除拷贝构造函数
    Logger(const Logger& other) = delete;

private:
    /// 将构造函数修饰为私有，防止用户擅自实例化对象
    Logger() = default;

private:
    /// 日志信息
    Information m_information;
    /// 日志输出地列表
    std::list<Appender> m_appenderList;
    /// 单例对象
    static std::shared_ptr<Logger> instance;
};


}   // namespace log
}   // namespace o7si