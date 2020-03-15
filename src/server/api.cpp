#include "api.h"

#include <iostream>

namespace o7si
{
namespace server
{
    

// ----------------------------------------------------------------------------

bool help(const std::string& in, std::string& out)
{
    out = "1 + 1 = 2";
    return true;      
}

bool stage_list(const std::string& in, std::string& out)
{
    auto mapping = o7si::seda::StageManager::Instance()->getMapping();    

    Json::Value array;
    for (const auto& item : mapping)
    {
        auto stage = item.second;

        Json::Value obj;
        // 名称
        obj["name"] = stage->getName();
        // 短名称
        obj["short_name"] = stage->getShortName();
        // 事件队列的长度
        obj["event_queue_size"] = (int)stage->getEventQueueSize();
        // 线程池的容量
        obj["thread_pool_capacity"] = (int)stage->getThreadPoolCapacity();
        // 性能监控器的容量
        obj["performeter_capacity"] = (int)stage->getPerformeterCapacity();
        // 可能的后续状态数
        obj["next_state_number"] = (int)stage->getNextStateNumber();
        // 是否运行在系统中
        obj["is_run"] = stage->isRun(); 
        
        array.append(obj);
    }
    Json::Value root;
    root["data"] = array;
    root["error_code"] = 0;

    Json::FastWriter writer;
    out = writer.write(root);

    std::cout << out << std::endl;
    return true;
}

bool repeater(const std::string& in, std::string& out)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    out = in;
    return true;    
}

// ----------------------------------------------------------------------------

REGISTER_API(/help, help)
REGISTER_API(/stage/list, stage_list)
REGISTER_API(/repeater, repeater)

// ----------------------------------------------------------------------------


}   // namespace server    
}   // namespace o7si
