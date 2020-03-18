#include "api.h"


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

bool verify(const Json::Value& params)
{
    auto manager = WebServerManager::Instance();
    if (!manager->isAuth())
        return true;
    return params["authority"].asString() == manager->getAuthCode();
}

// ----------------------------------------------------------------------------

bool stage_list(const std::string& in, std::string& out)
{
    Json::Value params;
    bool ret = make_json(in, params);
    if (!ret)
        return false;

    if (!verify(params))
    {
        out = make_error_json(2, "permission denied");
        return true;    
    }

    Json::Value data = [&]
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
        return array;
    }();

    Json::Value root;
    root["data"] = data;
    root["error_code"] = 0;
    root["error_desc"] = "";

    return make_sequence(root, out);
}

bool stage_info(const std::string& in, std::string& out)
{
    Json::Value params;
    bool ret = make_json(in, params);
    if (!ret)
        return false;

    if (!verify(params))
    {
        out = make_error_json(2, "permission denied");
        return true;    
    }

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

    auto stage = o7si::seda::StageManager::Instance()->doLogin(stage_name); 
    
    Json::Value data;
    data["name"] = stage->getName();
    data["short_name"] = stage->getShortName(); 
    data["event_queue_size"] = (int)stage->getEventQueueSize();
    data["thread_pool_name"] = stage->getThreadPoolName();
    data["thread_pool_capacity"] = (int)stage->getThreadPoolCapacity();
    data["performeter_name"] = stage->getPerformeterName();
    data["performeter_capacity"] = (int)stage->getPerformeterCapacity();
    data["next_state_number"] = (int)stage->getNextStateNumber();
    data["next_state_info"] = [&]
    {
        auto conver_mapping = stage->next();

        Json::Value array;
        for (const auto& item : conver_mapping)
            array.append(item.first + " -> " + item.second->getName());

        return array;
    }();
    data["is_run"] = stage->isRun();
    data["longest_dura"] = stage->getLongestDura();
    data["longest_wait_dura"] = stage->getLongestWaitDura();
    data["longest_exec_dura"] = stage->getLongestExecDura();
    data["sum_dura"] = stage->getSumDura();
    data["sum_wait_dura"] = stage->getSumWaitDura();
    data["sum_exec_dura"] = stage->getSumExecDura();
    data["avg_dura"] = stage->getAvgDura();
    data["avg_wait_dura"] = stage->getAvgWaitDura();
    data["avg_exec_dura"] = stage->getAvgExecDura();
    data["lastest_dura_list"] = [&]
    {
        auto dura_list = stage->getLastestDuraList();
        
        Json::Value array;
        for (const auto& item : dura_list) 
            array.append(item);   
        return array;
    }();
    data["lastest_wait_dura_list"] = [&]
    {
        auto dura_list = stage->getLastestWaitDuraList();
        
        Json::Value array;
        for (const auto& item : dura_list) 
            array.append(item);   
        return array;
    }();
    data["lastest_exec_dura_list"] = [&]
    {
        auto dura_list = stage->getLastestExecDuraList();
        
        Json::Value array;
        for (const auto& item : dura_list) 
            array.append(item);   
        return array;
    }();
    data["task_count"] = (int)stage->getTaskCount();

    Json::Value root;
    root["data"] = data;
    root["error_code"] = 0;
    root["error_desc"] = "";
    
    return make_sequence(root, out);
}

bool stage_update(const std::string& in, std::string& out)
{
    Json::Value params;
    bool ret = make_json(in, params);
    if (!ret)
        return false;

    if (!verify(params))
    {
        out = make_error_json(2, "permission denied");
        return true;    
    }
        
    // 没有携带 stage_name 参数
    if (!params.isMember("stage_name"))
    {
        out = make_error_json(1, "missing parameter");
        return true;
    }

    // 没有携带 key 和 value 参数
    if (!params.isMember("key") && !params.isMember("value"))
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

    auto stage = o7si::seda::StageManager::Instance()->doLogin(stage_name); 
    std::string key = params["key"].asString();
    if (key == "name")
    {
        std::string value = params["value"].asString();
        bool ret = stage->setName(value);
        if (!ret)
        {
            out = make_error_json(103, "stage name repeat");
            return true;    
        }
    }
    else if (key == "short_name")
    {
        std::string value = params["value"].asString();
        stage->setShortName(value);    
    }
    else if (key == "thread_pool_name")
    {
        std::string value = params["value"].asString();    
        stage->setThreadPoolName(value);
    }
    else if (key == "thread_pool_capacity")
    {
        int value = std::stoi(params["value"].asString());
        stage->setThreadPoolCapacity(value);    
    }
    else if (key == "performeter_name")
    {
        std::string value = params["value"].asString();
        stage->setPerformeterName(value);    
    }
    else if (key == "performeter_capacity")
    {
        int value = std::stoi(params["value"].asString());
        stage->setPerformeterCapacity(value);    
    }
    else
    {
        out = make_error_json(102, "not find key");
        return true;
    }

    Json::Value root;
    root["data"] = "update success";
    root["error_code"] = 0;
    root["error_desc"] = "";

    return make_sequence(root, out);
}

bool repeater(const std::string& in, std::string& out)
{
    int rand = o7si::random::randint(1000, 5000);
    std::this_thread::sleep_for(std::chrono::milliseconds(rand));
    out = in;
    return true;    
}

// ----------------------------------------------------------------------------

REGISTER_API(/stage/list, stage_list)
REGISTER_API(/stage/info, stage_info)
REGISTER_API(/stage/update, stage_update)
REGISTER_API(/repeater, repeater)

// ----------------------------------------------------------------------------


}   // namespace server    
}   // namespace o7si
