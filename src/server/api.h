/******************************************************************************
 * File: api.h
 * Description: 服务接口。
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <jsoncpp/json/json.h>

#include "api_manager.h"


namespace o7si
{
namespace server
{

// 获取帮助文档
bool help(const std::string& in, std::string& out);

// 复读机
bool repeater(const std::string& in, std::string& out);

}   // namespace server     
}   // namespace o7si
