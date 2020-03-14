/******************************************************************************
 * File: api.h
 * Description: 服务接口。
 * Author: o7si
 *****************************************************************************/
#pragma once


#include "api_manager.h"


namespace o7si
{
namespace server
{


// 复读机
bool repeater(const std::string& in, std::string& out);


}   // namespace server     
}   // namespace o7si
