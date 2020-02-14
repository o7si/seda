#include "pch.h"
#include "log.h"
#include "config.h"

#include "seda/stage.h"
#include "stage/TestStage.hpp"

int main(int argc, char* argv[])
{
    REGISTER_STAGE(CountStage)
    REGISTER_STAGE(MaxMinStage)
    REGISTER_STAGE(MultiStage)
    REGISTER_STAGE(BinaryStage)
    
    o7si::config::load("/root/reps/seda/conf/test.conf");

    while (true)
    {
        auto count_stage = o7si::seda::StageManager::getInstance()->doLogin("CountStage");
        auto max_min_stage = o7si::seda::StageManager::getInstance()->doLogin("MaxMinStage");
        auto multi_stage = o7si::seda::StageManager::getInstance()->doLogin("MultiStage");
        auto binary_stage = o7si::seda::StageManager::getInstance()->doLogin("BinaryStage");

        count_stage->call({});
        max_min_stage->call({});
        multi_stage->call({});
        binary_stage->call({});

        count_stage->internal_state();
        max_min_stage->internal_state();
        multi_stage->internal_state();
        binary_stage->internal_state();

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    return 0;
};
