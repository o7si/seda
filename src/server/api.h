/******************************************************************************
 * File: api.h
 * Description: 服务接口。
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <chrono>
#include <thread>

#include <jsoncpp/json/json.h>

#include "../seda/stage_manager.h"

#include "api_manager.h"


namespace o7si
{
namespace server
{


// 获取帮助文档
bool help(const std::string& in, std::string& out);

// 获取 Stage 列表 
bool stage_list(const std::string& in, std::string& out);

// 复读机
bool repeater(const std::string& in, std::string& out);


}   // namespace server     
}   // namespace o7si
