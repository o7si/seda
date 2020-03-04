#include "config.h"

namespace o7si
{
namespace config
{

// ------------------------------------------------------------------------------

std::shared_ptr<PathManager> PathManager::instance(new PathManager());

std::shared_ptr<PathManager> PathManager::Instance()
{
    return instance;    
}

void PathManager::set(const std::string& key, const std::string& value)
{
    m_paths[key] = value;    
}

std::string PathManager::get(const std::string& key, const std::string& def)
{
    if (!exist(key))
        return def + "/";
    return m_paths[key] + "/"; 
}

void PathManager::remove(const std::string& key)
{
    if (!exist(key))
        return;    
    m_paths.erase(m_paths.find(key));
}

bool PathManager::exist(const std::string& key)
{
    return has(key); 
}

bool PathManager::has(const std::string& key)
{
    return m_paths.find(key) != m_paths.end(); 
}

// ------------------------------------------------------------------------------

void load(const std::string& filename)
{
    // 载入配置文件
    LOG_INFO_SYS << "Loading...(config)";
    YAML::Node config = YAML::LoadFile(filename);    
    
    // 载入路径配置
    LOG_INFO_SYS << "Loading...(config.path)";
    YAML::Node path_config = config["path"];
    path_config >> *o7si::config::PathManager::Instance();
    LOG_INFO_SYS << "Complete(config.path)";

    // 日志模块的配置
    LOG_INFO_SYS << "Loading...(config.log)";
    YAML::Node log_config = config["log"];
    log_config >> *o7si::log::LoggerManager::Instance();
    LOG_INFO_SYS << "Complete(config.log)";

    // Stage 的配置
    LOG_INFO_SYS << "Loading...(config.stage)";
    YAML::Node stage_config = config["stage"];
    stage_config >> *o7si::seda::StageManager::getInstance();
    LOG_INFO_SYS << "Complete(config.stage)";

    // 其它模块的配置
    // ...

    LOG_INFO_SYS << "Complete(config)";
}

// ------------------------------------------------------------------------------

void operator>>(const YAML::Node& yaml, o7si::config::PathManager& manager)
{
    // 读取每一组键值对
    for (auto iter = yaml.begin(); iter != yaml.end(); ++ iter)
    {
        std::string key = iter->first.as<std::string>();
        std::string value = iter->second.as<std::string>();
        
        manager.set(key, value);   
    }
}

void operator>>(const YAML::Node& yaml, o7si::log::LoggerManager& manager)
{
    for (auto i = yaml.begin(); i != yaml.end(); ++ i)
    {
        // 日志名称
        std::string name = i->first.as<std::string>();
        // 日志级别
        std::string level = i->second["level"].as<std::string>();
        // 注册一个日志用户
        auto user = manager.doRegister(name, o7si::log::GenLevelFrom(level));
        // 日志的输出地
        YAML::Node appenders = i->second["appender"];
        for (auto j = appenders.begin(); j != appenders.end(); ++ j)
        {
            // 输出地类型
            std::string type = o7si::utils::to_lower((*j)["type"].as<std::string>());
            // 日志输出格式
            std::string pattern = (*j)["pattern"].as<std::string>();

            // 控制台输出地
            if (type == "consoleappender")
            {
                user->add_appender(
                    std::make_shared<o7si::log::ConsoleAppender>(
                        std::make_shared<o7si::log::Layout>(pattern)  
                    )
                ); 
            }
            // 文件输出地
            else if (type == "fileappender")
            {
                std::string path = PathManager::Instance()->get("log", "/tmp/");
                std::string file = path + (*j)["file"].as<std::string>();
                user->add_appender(
                    std::make_shared<o7si::log::FileAppender>(
                        file, std::make_shared<o7si::log::Layout>(pattern) 
                    )
                );     
            }
            // 其它输出地
            else
            {
            } 
        }
    }
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
