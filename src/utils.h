#pragma once

#include "pch.h"

namespace o7si
{
namespace utils
{

/// 将 thread::id 转换为 uint64_t
uint64_t convertThreadId(std::thread::id threadId);

}   // namespace utils
}   // o7si
