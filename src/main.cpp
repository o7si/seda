#include "pch.h"
#include "config.h"
#include "seda/stage.h"
#include "stage/test_stage.hpp"
#include "seda/thread_pool.hpp"

int main(int argc, char* argv[])
{
    using namespace o7si::seda;
    ThreadPool thread_pool(10);
    thread_pool.init();

    std::vector<std::future<std::string>> vec;
    for (int i = 0; i < 1000; ++ i)
        vec.emplace_back(thread_pool.call([i](int a, int b)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            return std::to_string(a + b + i);    
        }, 0, 0));

    for (int i = 0; i < vec.size(); ++ i)
        LOG_DEBUG << vec[i].get();    

    return 0;    
}

int _main(int argc, char* argv[])
{
    // 注册 Stage
    REGISTER_STAGE(CountStage)
    REGISTER_STAGE(MaxMinStage)
    REGISTER_STAGE(MultiStage)
    REGISTER_STAGE(BinaryStage)
    // 载入配置
    o7si::config::load("/root/reps/seda/conf/test.conf");
    // 模拟
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
