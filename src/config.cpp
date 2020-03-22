#include "config.h"


namespace o7si
{
namespace config
{

// ----------------------------------------------------------------------------

bool exist(const YAML::Node& yaml, const std::string& key)
{
    std::stringstream stream;     
    for (size_t i = 0; i < key.size(); ++ i)
        stream << (key[i] == '.' ? ' ' : key[i]);

    std::vector<std::string> vec;
    std::string tmp;
    while (stream >> tmp)
        vec.push_back(tmp);

    YAML::Node node = YAML::Clone(yaml);
    for (const auto& item : vec)
        node = node[item];

    return node.Type() != YAML::NodeType::Undefined;
}

// ----------------------------------------------------------------------------

void load(const std::string& filename)
{
    try
    {
        // 载入配置文件
        LOG_INFO_SYS << "Loading...(config)";
        YAML::Node config = YAML::LoadFile(filename);    

        // 系统配置（webserver）
        LOG_INFO_SYS << "Loading...(config.sys.webserver)"; 
        if (exist(config, "system.webserver"))
        {
            YAML::Node webserver_config = config["system"]["webserver"];
            webserver_config >> *o7si::server::WebServerManager::Instance();      
        }
        LOG_INFO_SYS << "Complete(config.sys.webserver)"; 

        // 日志模块的配置
        LOG_INFO_SYS << "Loading...(config.log)";
        if (exist(config, "log"))
        {
            YAML::Node log_config = config["log"];
            log_config >> *o7si::log::LoggerManager::Instance();
        }
        LOG_INFO_SYS << "Complete(config.log)";

        // Stage 的配置
        LOG_INFO_SYS << "Loading...(config.stage)";
        if (exist(config, "stage"))
        {
            YAML::Node stage_config = config["stage"];
            stage_config >> *o7si::seda::StageManager::Instance();
        }
        LOG_INFO_SYS << "Complete(config.stage)";

        // 其它模块的配置
        // ...

        LOG_INFO_SYS << "Complete(config)";
    }
    catch (...)
    {
        LOG_ERROR_SYS << "Config Error!";
       throw std::logic_error(""); 
    }
}

// ----------------------------------------------------------------------------

void operator>>(const YAML::Node& yaml,
                o7si::server::WebServerManager& manager)
{
    if (yaml.Type() == YAML::NodeType::Undefined)
        return;

    // is_authority 的默认值为 true
    // 当配置文件中未指定时，使用默认值
    manager.isAuth(exist(yaml, "is_authority") ?
        yaml["is_authority"].as<bool>() : true
    );

    manager.genAuthCode();    

    if (exist(yaml, "authority_code_path"))
    {
        std::string path = yaml["authority_code_path"].as<std::string>();
        manager.isSave(true);    
        manager.setAuthPath(path);
        manager.save();
    }
}

void operator>>(const YAML::Node& yaml, 
                o7si::log::LoggerManager& manager)
{
    if (yaml.Type() == YAML::NodeType::Undefined)
        return;

    for (auto i = yaml.begin(); i != yaml.end(); ++ i)
    {
        // 日志的用户名称
        std::string name = i->first.as<std::string>();

        // 禁止调整系统用户
        if (name == LOG_SYSTEM_USER)
            continue;

        // 如果未指定日志级别，则默认为 DEBUG 级别
        auto log_user = manager.doRegister(name, o7si::log::GenLevelFrom(
            exist(i->second, "level") ? 
            i->second["level"].as<std::string>() : "DEBUG"
        ));

        // 输出地
        YAML::Node appenders = i->second["appender"];
        for (auto j = appenders.begin(); j != appenders.end(); ++ j)
        {
            // 输出地类型
            std::string type = 
                o7si::utils::to_lower((*j)["type"].as<std::string>());
            // 输出格式
            std::string pattern = exist(*j, "pattern") ? 
                (*j)["pattern"].as<std::string>() : "default";

            // 控制台输出地
            if (type == "consoleappender")
            {
                log_user->add_appender(
                    std::make_shared<o7si::log::ConsoleAppender>(
                        std::make_shared<o7si::log::Layout>(pattern)  
                    )
                ); 
            }
            // 文件输出地
            else if (type == "fileappender")
            {
                std::string file = (*j)["file"].as<std::string>();
                log_user->add_appender(
                    std::make_shared<o7si::log::FileAppender>(
                        file, std::make_shared<o7si::log::Layout>(pattern) 
                    )
                );     
            }
            // 其它输出地
            // ...
        }
    }
}

void operator>>(const YAML::Node& yaml, 
                o7si::seda::StageManager& manager)
{
    if (yaml.Type() == YAML::NodeType::Undefined)
        return;

    for (auto i = yaml.begin(); i != yaml.end(); ++ i)
    {
        // 登录
        std::string stage_name = i->first.as<std::string>();
        auto stage = 
            o7si::seda::StageManager::Instance()->doLogin(stage_name);

        // 线程池容量
        size_t thread_pool_capacity = exist(i->second, "thread_capacity") ?
            i->second["thread_capacity"].as<size_t>() : 1;
        stage->setThreadPoolCapacity(thread_pool_capacity);

        // 调整状态
        stage->run();

        // 后续状态
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
}

// ----------------------------------------------------------------------------

}   // namespace config
}   // namespace o7si
