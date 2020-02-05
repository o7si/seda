#include <iostream>
#include <memory>
#include <thread>

#include "log.h"
#include "config.h"
#include "seda.h"
#include "stage.h"

int main(int argc, char* argv[])
{
    o7si::config::load("/root/reps/seda/conf/test.conf");
    
    REGISTER_STAGE(StageA)
    REGISTER_STAGE(StageB)
    REGISTER_STAGE(StageC)

    int count = 3;
    while (count --)
    {
        LOG_DEBUG << "count = " << count;
        LOG_INFO << "count = " << count;
        LOG_WARN << "count = " << count;
        LOG_ERROR << "count = " << count;
        LOG_FATAL << "count = " << count;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    auto mapping = o7si::seda::StageManager::getInstance()->registerMapping();
    for (auto iter = mapping.begin(); iter != mapping.end(); ++ iter)
    {
        LOG_DEBUG << iter->first;     
    }
    return 0;
};
