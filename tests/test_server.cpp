#include <server/web_server.h>
#include <config.h>
#include "stage/test01.h"

int main(int argc, char* argv[])
{
    o7si::config::load("/root/reps/seda/conf/test.conf");
    auto stage4 = o7si::seda::StageManager::Instance()->doLogin("Stage4");

    o7si::log::LoggerManager::Instance()->login_behavior(1);

    o7si::server::WebServer server(4, atoi(argv[1]), "/root/reps/seda/web");
    server.start();
    std::this_thread::sleep_for(std::chrono::seconds(2));    
    server.stop();

    server.setProtocol(6);
    server.restart();

    auto stage1 = o7si::seda::StageManager::Instance()->doLogin("Stage1");
    while (true)
    {
        stage1->call(0);        
        stage1->call(0);        
        stage1->call(0);        

        std::this_thread::sleep_for(std::chrono::seconds(2));


        auto stage = o7si::seda::StageManager::Instance()->doLogin("HelloWorld");
        if (stage == nullptr)
            continue;
        std::cout << stage->getName() << std::endl;
    }

    return 0;    
}
