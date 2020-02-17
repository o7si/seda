#include "stage_manager.h"

namespace o7si
{
namespace seda
{

std::shared_ptr<StageManager> StageManager::instance(new StageManager);


/// 注册 Stage 到管理类
std::shared_ptr<Stage> StageManager::doRegister(const std::string& name, std::shared_ptr<Stage> stage)
{
    // 名称为 name 的 Stage 没有被注册
    if (mapping.find(name) == mapping.end())
    {
        // 注册成功
        mapping[name] = stage;
        LOG_INFO << "register success: " << name;
        return stage;
    }
    // 注册失败
    LOG_INFO << "register failure: " << name;
    return nullptr;
}

/// 从管理类中获取 Stage 
std::shared_ptr<Stage> StageManager::doLogin(const std::string& name)
{
    // 名称为 name 的 Stage 已经被注册
    if (mapping.find(name) != mapping.end())
    {
        // 登录成功
        LOG_INFO << "login success: " << name;
        return mapping[name];
    }
    // 登录失败
    LOG_INFO << "login failure: " << name;
    return nullptr;
}

}    
}