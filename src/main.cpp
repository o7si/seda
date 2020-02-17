#include "pch.h"
#include "config.h"
#include "seda/stage.h"
#include "seda/thread_pool.hpp"
#include "seda/stage_manager.h"
#include "stage/test_stage.hpp"

int main()
{
    using namespace o7si::seda;
    REGISTER_STAGE(StageOne)

    auto login_stage = StageManager::getInstance()->doLogin("StageOne");
    LOG_DEBUG << login_stage->getName();

    std::function<boost::any(boost::any&&)> func = [login_stage](boost::any&& args)
    {
        return login_stage->handler(std::forward<boost::any>(args));    
    };

    login_stage->bind(std::move(func));

    login_stage->call(1);
    return 0;    
}
