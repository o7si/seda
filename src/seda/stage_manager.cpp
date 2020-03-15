#include "stage_manager.h"


namespace o7si
{
namespace seda
{


std::shared_ptr<StageManager> StageManager::Instance()
{
    static std::shared_ptr<StageManager> instance(new StageManager());
    return instance;    
}

std::shared_ptr<Stage> 
StageManager::doRegister(const std::string& name, std::shared_ptr<Stage> stage)
{
    // 名称为 name 的 Stage 没有被注册
    if (!has(name))
    {
        // 注册成功
        m_mapping[name] = stage;
        LOG_INFO_SYS << "register success: " << name;
        return stage;
    }
    // 注册失败
    LOG_INFO_SYS << "register failure: " << name;
    return nullptr;
}

std::shared_ptr<Stage> 
StageManager::doLogin(const std::string& name)
{
    // 名称为 name 的 Stage 已经被注册
    if (has(name))
    {
        // 登录成功
        LOG_INFO_SYS << "login success: " << name;
        return m_mapping[name];
    }
    // 登录失败
    LOG_INFO_SYS << "login failure: " << name;
    return nullptr;
}


}    
}
