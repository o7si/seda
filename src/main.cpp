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

    auto entrance = o7si::seda::StageManager::getInstance()->doLogin("CountStage");
    while (true)
    {
        entrance->call({
            { "value", "aaaaabbbbcccdde" }
        });   
        auto state = entrance->internal_state();
        for (auto iter = state.begin(); iter != state.end(); ++ iter)
        {
            LOG_DEBUG << iter->first << " " << iter->second;
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    return 0;
};
