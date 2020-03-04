#include <config.h>
#include <log.h>

#include "stage/test01.h"

void test_config_path()
{
    auto pm = o7si::config::PathManager::Instance();
    LOG_INFO_SYS << "log -> " << pm->get("log", "unknown"); 
    LOG_INFO_SYS << "web -> " << pm->get("web", "unknown"); 
}

void test_config_log()
{
    LOG_DEBUG(system) << "debug";
    LOG_INFO(system) << "info";
    LOG_WARN(system) << "warn";
    LOG_ERROR(system) << "error";
    LOG_FATAL(system) << "fatal";

    LOG_DEBUG(my-log-1) << "debug"; 
    LOG_INFO(my-log-1) << "info"; 
    LOG_WARN(my-log-1) << "warn"; 
    LOG_ERROR(my-log-1) << "error"; 
    LOG_FATAL(my-log-1) << "fatal"; 

    LOG_DEBUG(my-log-2) << "debug"; 
    LOG_INFO(my-log-2) << "info"; 
    LOG_WARN(my-log-2) << "warn"; 
    LOG_ERROR(my-log-2) << "error"; 
    LOG_FATAL(my-log-2) << "fatal"; 
}

void test_config_stage()
{
    using namespace o7si::seda;
    auto stage1 = StageManager::Instance()->doLogin("Stage1");
    auto stage2 = StageManager::Instance()->doLogin("Stage2");
    auto stage3 = StageManager::Instance()->doLogin("Stage3");
    auto stage4 = StageManager::Instance()->doLogin("Stage4");

    auto stage1_mapping = stage1->next(); 
    for (const auto& item : stage1_mapping)
        LOG_INFO_SYS << item.first << " " << item.second->getName();
    auto stage2_mapping = stage2->next(); 
    for (const auto& item : stage2_mapping)
        LOG_INFO_SYS << item.first << " " << item.second->getName();
    auto stage3_mapping = stage3->next(); 
    for (const auto& item : stage3_mapping)
        LOG_INFO_SYS << item.first << " " << item.second->getName();
    auto stage4_mapping = stage4->next(); 
    for (const auto& item : stage4_mapping)
        LOG_INFO_SYS << item.first << " " << item.second->getName();
}

void test_replace_layout()
{
    using namespace o7si::log;
    auto user = LoggerManager::Instance()->doLogin("system");
    LOG_INFO_SYS << "before";
    std::list<std::shared_ptr<Appender>> apps = user->appenders();
    std::shared_ptr<Appender> app = *apps.begin();
    app->layout(std::make_shared<Layout>("%u|%u|%u %c")); 
    LOG_INFO_SYS << "end";
}

int main(int argc, char* argv[])
{
    o7si::config::load("/root/reps/seda/conf/test.conf");
    //test_config_path();    
    test_config_log();
    //test_config_stage();
    test_replace_layout();
    return 0;   
}
