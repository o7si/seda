#include "pch.h"
#include "config.h"
#include "seda/stage.h"
#include "seda/thread_pool.hpp"
#include "seda/stage_manager.h"
#include "stage/test_stage.hpp"
#include "seda/macro.hpp"

int main()
{
    REGISTER_STAGE(StageOne)

    auto stage = LOGIN_STAGE(StageOne) 
    for (int i = 0; i < 100; ++ i)
    {
        stage->call(i);
        std::this_thread::sleep_for(std::chrono::seconds(1));    
    }
    return 0;    
}
