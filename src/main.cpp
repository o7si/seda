#include <iostream>
#include <memory>
#include <thread>

#include "log.h"
#include "config.h"

int main(int argc, char* argv[])
{
    o7si::config::load("/reps/seda/conf/test.conf");

    int count = 5;
    while (count --)
    {
        LOG_DEBUG << "count = " << count;
        LOG_INFO << "count = " << count;
        LOG_WARN << "count = " << count;
        LOG_ERROR << "count = " << count;
        LOG_FATAL << "count = " << count;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
};
