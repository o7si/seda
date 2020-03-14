/******************************************************************************
 * File: random.h
 * Description: 随机库。
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>


namespace o7si
{
namespace random
{
    

// 生成 UUID
std::string uuid();


}   // namespace random
}   // namespace o7si
