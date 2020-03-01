#include "config.h"

namespace o7si
{
namespace config
{

// ------------------------------------------------------------------------------

void load(const std::string& filename)
{
    // 载入配置文件
    //LOG_INFO << "Loading...(config)";
    YAML::Node config = YAML::LoadFile(filename);    
    
    // 日志模块的配置
    //LOG_INFO << "Loading...(config.log)";
    YAML::Node log_config = config["log"];
    //log_config >> *o7si::log::Logger::getInstance();
    //LOG_INFO << "Load complete(config.log)";

    // Stage 的配置
    //LOG_INFO << "Loading...(config.stage)";
    YAML::Node stage_config = config["stage"];
    stage_config >> *o7si::seda::StageManager::getInstance();
    //LOG_INFO << "Load complete(config.stage)";

    // 其它模块的配置
    // ...

    //LOG_INFO << "Load complete(config)";
}

// ------------------------------------------------------------------------------

void operator>>(const YAML::Node& yaml, o7si::log::Logger& logger)
{
    /*
    // 清空输出地(默认有一个控制台输出地)
    o7si::log::Logger::getInstance()->clearAppenders();

    // 设置全局日志级别
    YAML::Node level = yaml["level"];
    logger.setLevel(o7si::log::FromString(level.as<std::string>()));

    // 设置日志输出地
    YAML::Node appender = yaml["appender"];
    for (auto iter = appender.begin(); iter != appender.end(); ++ iter)
    {
        std::string type = (*iter)["type"].as<std::string>();
        std::string pattern = (*iter)["pattern"].as<std::string>();

        // 控制台输出地
        if (type == "ConsoleAppender")
        {
            logger.addAppender(
                std::make_shared<o7si::log::ConsoleAppender>(
                    std::make_shared<o7si::log::Layout>(std::move(pattern))      
            ));
            continue;
        }

        // 文件输出地
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

        // other
    } 
    */
    // 异常
    // throw
}

void operator>>(const YAML::Node& yaml, o7si::seda::StageManager& manager)
{
    for (auto i = yaml.begin(); i != yaml.end(); ++ i)
    {
        // Stage 的名称
        std::string stage_name = i->first.as<std::string>();
        // 登录
        auto stage = o7si::seda::StageManager::getInstance()->doLogin(stage_name);

        // Stage 的最大允许线程数
        size_t thread_pool_capacity = i->second["max_thread"].as<size_t>();
        stage->setThreadPoolCapacity(thread_pool_capacity);
        stage->init();

        // Stage 的后续状态
        YAML::Node state_table = i->second["state"];
        for (auto j = state_table.begin(); j != state_table.end(); ++ j)
        {
            // 状态名
            std::string state_name = j->first.as<std::string>();
            // 后续 Stage    
            std::string next_stage = j->second.as<std::string>();

            stage->next(state_name, next_stage);
        }
    }

    // 异常
    // throw
}

// ------------------------------------------------------------------------------

}   // namespace config
}   // namespace o7si
