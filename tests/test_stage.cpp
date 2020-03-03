#include <config.h>
#include "stage/test01.h"

void test_stage()
{
    using namespace o7si::seda;
    auto stage1 = StageManager::getInstance()->doLogin("Stage1");
    auto stage2 = StageManager::getInstance()->doLogin("Stage2");
    auto stage3 = StageManager::getInstance()->doLogin("Stage3");
    auto stage4 = StageManager::getInstance()->doLogin("Stage4");
    
    o7si::log::LoggerManager::Instance()->login_behavior(1);
    LOG_DEBUG(test) << stage1->getThreadPoolCapacity();
    LOG_DEBUG(test) << stage2->getThreadPoolCapacity();
    LOG_DEBUG(test) << stage3->getThreadPoolCapacity();
    LOG_DEBUG(test) << stage4->getThreadPoolCapacity();

    while (true)
    {
        stage1->call(0);   
        stage1->call(0);   
        stage1->call(0);   
        stage1->call(0);   
        stage1->call(0);   
        std::this_thread::sleep_for(std::chrono::seconds(3));

        stage1->performeter_internal_state();
    }
}

int main(int argc, char* argv[])
{
    o7si::config::load("/root/reps/seda/conf/test.conf");
    test_stage();
    return 0;    
}
