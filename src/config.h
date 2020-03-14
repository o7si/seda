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

class PathManager
{
public:
    /// 添加一组映射
    void set(const std::string& key, 
             const std::string& value);

    /// 获取 key 对应的 value
    std::string get(const std::string& key, 
                    const std::string& def);

    /// 移除一组映射
    void remove(const std::string& key);
    
    /// 判定 key 是否存在
    bool exist(const std::string& key) const;

    /// 判定 key 是否存在
    bool has(const std::string& key) const;

public:
    /// 获取 PathManager 的单例对象
    static std::shared_ptr<PathManager> Instance();
    
    /// 删除拷贝构造函数，防止用户拷贝对象
    PathManager(const PathManager& other) = delete;

    /// 删除移动构造函数，防止用户使用移动语义
    PathManager(PathManager&& other) = delete;

private:
    /// 将构造函数修饰为私有，防止用户擅自实例化对象
    PathManager() = default;

    /// 映射表
    std::unordered_map<std::string, std::string> m_paths; 
    /// 单例对象
    static std::shared_ptr<PathManager> instance; 
};

/// 载入配置
void load(const std::string& filename);

void operator>>(const YAML::Node& yaml, o7si::config::PathManager& manager);
void operator>>(const YAML::Node& yaml, o7si::log::LoggerManager& manager);
void operator>>(const YAML::Node& yaml, o7si::seda::StageManager& manager);

}   // namespace config
}   // namespace o7si
