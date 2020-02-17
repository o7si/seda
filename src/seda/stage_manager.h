#pragma once

#include "stage.h"

namespace o7si
{
namespace seda
{
    
/// 前置声明
class Stage;

/// Stage 的管理类(单例工厂)
class StageManager
{
public:
    /// 注册 Stage 到管理类
    std::shared_ptr<Stage> doRegister(const std::string& name, std::shared_ptr<Stage> stage);

    /// 从管理类中获取 Stage 
    std::shared_ptr<Stage> doLogin(const std::string& name);

public:
    /// 获取 StageManager 的单例对象
    static std::shared_ptr<StageManager> getInstance()
    {
        return instance;    
    }

    /// 删除拷贝构造函数，防止用户拷贝该对象
    StageManager(const StageManager& other) = delete;
    /// 删除移动构造函数，防止用户使用移动语义
    StageManager(const StageManager&& other) = delete;

private:
    /// 将构造函数修饰为私有，防止用户擅自实例化对象
    StageManager() = default;

    /// 字符串和 Stage 的映射关系
    std::unordered_map<std::string, std::shared_ptr<Stage>> mapping;
    /// 单例对象
    static std::shared_ptr<StageManager> instance;
};

}   // namespace seda    
}   // namespace o7si
