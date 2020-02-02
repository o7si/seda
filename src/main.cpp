#include <iostream>
#include <memory>
#include <thread>
#include "log.h"

#include <yaml-cpp/yaml.h>

void operator>>(const YAML::Node& yaml, o7si::log::Logger& logger)
{
    YAML::Node level = yaml["level"];
    logger.setLevel(o7si::log::FromString(level.as<std::string>()));

    YAML::Node appender = yaml["appender"];
    for (auto iter = appender.begin(); iter != appender.end(); ++ iter)
    {
        std::string type = (*iter)["type"].as<std::string>();
        std::string pattern = (*iter)["pattern"].as<std::string>();

        if (type == "ConsoleAppender")
        {
            logger.addAppender(
                std::make_shared<o7si::log::ConsoleAppender>(
                    std::make_shared<o7si::log::Layout>(std::move(pattern))      
            ));
            continue;
        }
        if (type == "FileAppender")
        {
            std::string file = (*iter)["file"].as<std::string>();
            logger.addAppender(
                std::make_shared<o7si::log::FileAppender>(
                    std::move(file), 
                    std::make_shared<o7si::log::Layout>(std::move(pattern))
            ));
            continue;
        }

        // 异常
        // throw
    } 
}

int main(int argc, char* argv[])
{
    YAML::Node yaml = YAML::LoadFile("/reps/seda/conf/test.conf");
    YAML::Node log = yaml["log"];
    log >> *o7si::log::Logger::getInstance();
    
    int count = 10;
    while (count --)
    {
        LOG_DEBUG << "count = " << count;
        LOG_INFO << "count = " << count;
        LOG_WARN << "count = " << count;
        LOG_ERROR << "count = " << count;
        LOG_FATAL << "count = " << count;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
};
