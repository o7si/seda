#include "api.h"

#include <iostream>

namespace o7si
{
namespace server
{
    
// ----------------------------------------------------------------------------

bool make_json(const std::string& in, Json::Value& out)
{
    Json::Reader reader;
    return reader.parse(in, out);     
}

bool make_sequence(const Json::Value& in, std::string& out)
{
    try
    {
        Json::FastWriter writer;
        out = writer.write(in);    
        return true;
    }
    catch (...)
    {
        return false;    
    }
}

std::string make_error_json(int error_code, const std::string& error_desc)
{
    std::stringstream stream;
    stream << "{";
    stream << "\"" << "error_code" << "\": " << error_code << ", ";
    stream << "\"" << "error_desc" << "\": \"" << error_desc << "\""; 
    stream << "}";
    return stream.str();
}

// ----------------------------------------------------------------------------

bool help(const std::string& in, std::string& out)
{
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
        obj["name"] = stage->getName();
        obj["short_name"] = stage->getShortName();
        obj["event_queue_size"] = (int)stage->getEventQueueSize();
        obj["thread_pool_capacity"] = (int)stage->getThreadPoolCapacity();
        obj["performeter_capacity"] = (int)stage->getPerformeterCapacity();
        obj["next_state_number"] = (int)stage->getNextStateNumber();
        obj["is_run"] = stage->isRun(); 
        
        array.append(obj);
    }

    Json::Value root;
    root["data"] = array;

    return make_sequence(root, out);
}

bool stage_info(const std::string& in, std::string& out)
{
    Json::Value params;
    bool ret = make_json(in, params);
    if (!ret)
        return false;

    // 没有携带 stage_name 参数
    if (!params.isMember("stage_name"))
    {
        out = make_error_json(1, "missing parameter");
        return true;
    }

    std::string stage_name = params["stage_name"].asString();
    if (!o7si::seda::StageManager::Instance()->has(stage_name))
    {
        out = make_error_json(101, "not find stage");    
        return true;
    }

    Json::Value root; 
    Json::Value data;

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
REGISTER_API(/stage/info, stage_info)
REGISTER_API(/repeater, repeater)

// ----------------------------------------------------------------------------


}   // namespace server    
}   // namespace o7si
