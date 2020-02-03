#include <yaml-cpp/yaml.h>
#include <memory>
#include <string>

#include "log.h"

namespace o7si
{
namespace config
{

/// 载入配置
void load(const std::string& filename);

/// 日志模块的配置
void operator>>(const YAML::Node& yaml, o7si::log::Logger& logger);

}   // namespace config
}   // namespace o7si