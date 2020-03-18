/******************************************************************************
 * File: config.h
 * Description: 载入配置。
 * Author: o7si
 *****************************************************************************/
#pragma once 

#include <sstream>
#include <string>

#include <yaml-cpp/yaml.h>


#include "log.h"
#include "seda/stage.h"
#include "server/web_server.h"


namespace o7si
{
namespace config
{

// 判断配置文件中是否存在某字段
bool exist(const std::string& key);

// 载入配置
void load(const std::string& filename);

// 系统配置（webserver）
void operator>>(const YAML::Node& yaml, 
                o7si::server::WebServerManager& manager);

// 日志配置
void operator>>(const YAML::Node& yaml, 
                o7si::log::LoggerManager& manager);

// Stage 的配置
void operator>>(const YAML::Node& yaml, 
                o7si::seda::StageManager& manager);

}   // namespace config
}   // namespace o7si
