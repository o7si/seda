#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/// 线程名称的最大长度
#define THREAD_NAME_MAX_LENGTH 16

namespace o7si
{
namespace utils
{

/// 获取当前的时间戳
time_t get_timestamp();

/// 获取线程 ID
uint64_t get_thread_id();

/// 设置线程名称
void set_thread_name(const std::string& name);

/// 获取线程名称
std::string get_thread_name();

/// 将字符串转化为大写
std::string to_upper(const std::string& str);

/// 将字符串转化为小写
std::string to_lower(const std::string& str);

/// 载入文件内容
std::string load_file(const std::string& filename);

}   // namespace utils
}   // o7si
