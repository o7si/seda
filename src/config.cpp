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
    // 清空输出地
    o7si::log::Logger::getInstance()->clearAppenders();

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
    /*
    for (auto i_iter = yaml.begin(); i_iter != yaml.end(); ++ i_iter)
    {
        std::string stage_name = i_iter->first.as<std::string>();
        auto stage = o7si::seda::StageManager::getInstance()->doLogin(stage_name); 

        LOG_DEBUG << stage_name;

        YAML::Node next = i_iter->second;
        for (auto j_iter = next.begin(); j_iter != next.end(); ++ j_iter)
        {
            std::string state_name = j_iter->first.as<std::string>();
            std::string next_stage = j_iter->second.as<std::string>();
            
            LOG_DEBUG << state_name << " " << next_stage;
            stage->setNext(state_name, next_stage);
        }
    }
    */
    // 异常
    // throw
}

// ------------------------------------------------------------------------------

}   // namespace config
}   // namespace o7si
