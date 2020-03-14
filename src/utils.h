/******************************************************************************
 * File: utils.h
 * Description: 提供相关的工具函数。
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


// 线程名称的最大长度
// 由于系统限制，线程名称不能超过 16 个字节
#define THREAD_NAME_MAX_LENGTH 16


namespace o7si
{
namespace utils
{

// 获取当前的时间戳
time_t get_timestamp();

// 获取线程 ID
uint64_t get_thread_id();

// 设置线程名称
void set_thread_name(const std::string& name);

// 获取线程名称
std::string get_thread_name();

// 将字符串转化为大写
std::string to_upper(const std::string& str);

// 将字符串转化为小写
std::string to_lower(const std::string& str);

}   // namespace utils
}   // o7si
