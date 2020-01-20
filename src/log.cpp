#include "log.h"

namespace o7si
{

const char* LogLevel::ToString(LogLevel::Level level)
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


LogLevel::Level LogLevel::FromString(const std::string& level)
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

}