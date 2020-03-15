/******************************************************************************
 * File: api.h
 * Description: 服务接口。
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <chrono>
#include <sstream>
#include <thread>

#include <jsoncpp/json/json.h>

#include "../seda/stage_manager.h"

#include "api_manager.h"

// 错误码定义
// 1    ->  missing parameter
//
//
// 101  -> not find stage  

namespace o7si
{
namespace server
{

// ----------------------------------------------------------------------------

// 根据字符串生成 JSON 对象
bool make_json(const std::string& in, Json::Value& out);

// 根据 JSON 对象生成字符串
bool make_sequence(const Json::Value& in, std::string& out);

// 当发生错误时，调用该方法，生成一个描述错误信息的 JSON 串
std::string make_error_json(int error_code, const std::string& error_desc);

// ----------------------------------------------------------------------------

// 获取帮助文档
bool help(const std::string& in, std::string& out);

// 获取 Stage 列表 
// 对于每个 Stage 而言，将返回以下信息：
// | 1 | name                 | 名称               |
// | 2 | short_name           | 短名称             |
// | 3 | event_queue_size     | 事件队列的长度     |
// | 4 | thread_pool_capacity | 线程池的容量       |
// | 5 | performeter_capacity | 性能监控器的容量   |
// | 6 | next_state_number    | 可能的后续状态数量 |
// | 7 | is_run               | 是否运行在系统中   |
bool stage_list(const std::string& in, std::string& out);

// 获取某个 Stage 的详细信息
// 对于该 Stage 而言，将返回以下信息：
// | 1  | name                   | 名称                                  |
// | 2  | short_name             | 短名称                                |
// | 3  | event_queue_size       | 事件队列的长度                        |
// | 4  | thread_pool_name       | 线程池的名称                          |
// | 5  | thread_pool_capacity   | 线程池的容量                          |
// | 6  | performeter_name       | 性能监控器的名称                      |
// | 7  | performeter_capacity   | 性能监控器的容量                      |
// | 8  | next_state_number      | 可能的后续状态数量                    |
// | 9  | next_state_info        | 可能的后续状态信息                    |
// | 10 | is_run                 | 是否运行在系统中                      |
// | 11 | longest_dura           | 最长耗时（线程等待 + 任务执行）       |
// | 12 | longest_wait_dura      | 最长耗时（线程等待）                  |
// | 13 | longest_exec_dura      | 最长耗时（任务执行）                  |
// | 14 | sum_dura               | 总耗时（线程等待 + 任务执行）         |
// | 15 | sum_wait_dura          | 总耗时（线程等待）                    |
// | 16 | sum_exec_dura          | 总耗时（任务执行）                    |   
// | 17 | avg_dura               | 平均耗时（线程等待 + 任务执行）       |
// | 18 | avg_wait_dura          | 平均耗时（线程等待）                  |
// | 19 | avg_exec_dura          | 平均耗时（任务执行）                  |
// | 20 | lastest_dura_list      | 最新的耗时列表（线程等待 + 任务执行） | 
// | 21 | lastest_wait_dura_list | 最新的耗时列表（线程等待）            |
// | 22 | lastest_exec_dura_list | 最新的耗时列表（任务执行）            | 
// | 23 | task_count             | 总执行次数                            |
bool stage_info(const std::string& in, std::string& out);

// 复读机
bool repeater(const std::string& in, std::string& out);


}   // namespace server     
}   // namespace o7si
