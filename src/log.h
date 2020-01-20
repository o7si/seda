#pragma once

#include <string>

namespace o7si
{

/// 日志级别
class LogLevel
{
public:
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
    static const char* ToString(LogLevel::Level level);

    /// 通过文本获取日志级别对象
    static LogLevel::Level FromString(const std::string& level);
};

}