#include "pch.h"
#include "log.h"
#include "config.h"

int main(int argc, char* argv[])
{
    o7si::config::load("/root/reps/seda/conf/test.conf");

    auto mapping = o7si::seda::StageManager::getInstance()->registerMapping();
    for (auto iter = mapping.begin(); iter != mapping.end(); ++ iter)
        LOG_DEBUG << iter->first << "#" << iter->second;     
    return 0;
};
