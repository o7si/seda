/******************************************************************************
 * File: stage_manager.h
 * Description: 管理项目中所定义的 Stage。
 * Author: o7si
 *****************************************************************************/
#pragma once

#include "stage.h"


namespace o7si
{
namespace seda
{
    

class Stage;

// Stage 的管理类(单例工厂)
class StageManager
{
public:
    using MapType = std::unordered_map<std::string, std::shared_ptr<Stage>>;
public:
    // 注册 Stage 到管理类
    std::shared_ptr<Stage> doRegister(const std::string& name, 
                                      std::shared_ptr<Stage> stage);

    // 从管理类中获取 Stage 
    std::shared_ptr<Stage> doLogin(const std::string& name);

    // 更改 Stage 的映射名
    bool doUpdate(const std::string& old_name,
                  const std::string& new_name);

    // 是否有名称为 name 的 Stage
    bool has(const std::string& name)
    {
        return m_mapping.find(name) != m_mapping.end();    
    }

    MapType getMapping() const
    {
        return m_mapping;
    }    

public:
    // 获取单例对象
    static std::shared_ptr<StageManager> Instance();

    // 删除拷贝构造函数，防止用户拷贝该对象
    StageManager(const StageManager& other) = delete;
    // 删除移动构造函数，防止用户使用移动语义
    StageManager(const StageManager&& other) = delete;

private:
    StageManager() = default;

    // 映射关系
    MapType m_mapping;
};


}   // namespace seda    
}   // namespace o7si
