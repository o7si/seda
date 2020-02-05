#include "pch.h"
#include "stage.h"
#include "log.h"
#include "config.h"

int main(int argc, char* argv[])
{
    REGISTER_STAGE(StageA)
    REGISTER_STAGE(StageB)
    REGISTER_STAGE(StageC)

    o7si::config::load("/root/reps/seda/conf/test.conf");

    auto mapping = o7si::seda::StageManager::getInstance()->registerMapping();
    for (auto iter = mapping.begin(); iter != mapping.end(); ++ iter)
    {
        LOG_DEBUG << iter->first;     
    }
    return 0;
};
