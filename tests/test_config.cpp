#include <config.h>
#include <log.h>
#include <seda/stage_manager.h>
#include <server/web_server.h>

//#include "test_stage.h"

void test_config_system()
{
    auto ptr = o7si::server::WebServerManager::Instance();
    std::cout << "is_auth = " << ptr->isAuth() << std::endl;
    std::cout << "auth_code = " << ptr->getAuthCode() << std::endl;
    std::cout << "is_save = " << ptr->isSave() << std::endl;
    std::cout << "auth_path = " << ptr->getAuthPath() << std::endl; 
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

    LOG_DEBUG(my-log-3) << "debug"; 
    LOG_INFO(my-log-3) << "info"; 
    LOG_WARN(my-log-3) << "warn"; 
    LOG_ERROR(my-log-3) << "error"; 
    LOG_FATAL(my-log-3) << "fatal"; 
}

void test_config_stage()
{
    auto mapping = o7si::seda::StageManager::Instance()->getMapping();
    for (const auto& kv : mapping)
    {
        std::cout << kv.first 
                  << "(" << kv.second->getThreadPoolCapacity() << ")" 
                  << std::endl;
        auto next = kv.second->next();
        for (const auto& item : next)
            std::cout << "\t" 
                      << item.first 
                      << " -> " 
                      << item.second->getName()
                      << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
        std::cout << "Please input filename..." << std::endl;
        std::cout << "e.g." << std::endl;
        std::cout << argv[0] << " test.conf" << std::endl;    
        return EXIT_FAILURE;
    }

    o7si::config::load(argv[1]);
    test_config_system();
    test_config_log();
    test_config_stage();

    return EXIT_SUCCESS;   
}
