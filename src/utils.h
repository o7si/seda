#pragma once

#include "pch.h"

/// 线程名称的最大长度
#define THREAD_NAME_MAX_LENGTH 16

namespace o7si
{
namespace utils
{

/// 获取线程 ID
uint64_t get_thread_id();

/// 设置线程名称
void set_thread_name(const std::string& name);

/// 获取线程名称
std::string get_thread_name();

}   // namespace utils
}   // o7si
