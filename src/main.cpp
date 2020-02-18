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
    for (int i = 0; i < 1000; ++ i)
    {
        stage->call(std::string("aaaabbbccd"));
        if (i % 3 == 0)
            stage->performeter_internal_state();
    }
    stage->performeter_internal_state();
    
    return 0;    
}
