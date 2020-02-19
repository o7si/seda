#include "pch.h"
#include "config.h"
#include "seda/stage.h"
#include "seda/thread_pool.hpp"
#include "seda/stage_manager.h"
#include "stage/test_stage.hpp"
#include "seda/macro.hpp"

int main()
{
    o7si::config::load("/root/reps/seda/conf/test.conf");

    srand(time(nullptr));
    auto stage = LOGIN_STAGE(CountStage);
    LOG_INFO << stage->getThreadPoolCapacity();
    for (int i = 0; i < 300; ++ i)
    {
        stage->call(std::string("aaaabbbccd"));
    }

    LOG_FATAL << "wait...";
    std::this_thread::sleep_for(std::chrono::seconds(15));
    stage->performeter_internal_state();
    
    return 0;    
}
