#include "config.h"
#include "log.h"

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

int main(int argc, char* argv[])
{
    o7si::config::load("/root/reps/seda/conf/test.conf");
    test_config_path();    
    test_config_log();
    return 0;   
}
