#include "config.h"

namespace o7si
{
namespace config
{

// ------------------------------------------------------------------------------

void load(const std::string& filename)
{
    // 载入配置文件
    YAML::Node config = YAML::LoadFile(filename);    
    
    // 日志模块的配置
    YAML::Node log_config = config["log"];
    log_config >> *o7si::log::Logger::getInstance();

    // Stage 的配置
    YAML::Node stage_config = config["stage"];
    stage_config >> *o7si::seda::StageManager::getInstance();

    // 其它模块的配置
    // ...
}

// ------------------------------------------------------------------------------

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

void operator>>(const YAML::Node& yaml, o7si::seda::StageManager& manager)
{
    for (auto i_iter = yaml.begin(); i_iter != yaml.end(); ++ i_iter)
    {
        LOG_DEBUG << i_iter->first;
        for (auto j_iter = i_iter->second.begin(); j_iter != i_iter->second.end(); ++ j_iter)
        {
            LOG_DEBUG << "\t" << j_iter->first << " :---: " << j_iter->second; 
        }
    }
}

// ------------------------------------------------------------------------------

}   // namespace config
}   // namespace o7si
