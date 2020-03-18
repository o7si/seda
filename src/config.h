/******************************************************************************
 * File: config.h
 * Description: 载入配置。
 * Author: o7si
 *****************************************************************************/
#pragma once 

#include <string>

#include <yaml-cpp/yaml.h>

#include "log.h"
#include "seda/stage.h"


namespace o7si
{
namespace config
{


/// 载入配置
void load(const std::string& filename);

void operator>>(const YAML::Node& yaml, o7si::log::LoggerManager& manager);
void operator>>(const YAML::Node& yaml, o7si::seda::StageManager& manager);

}   // namespace config
}   // namespace o7si
