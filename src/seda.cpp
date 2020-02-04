#include "seda.h"

namespace o7si
{
namespace seda
{

// ------------------------------------------------------------------------------

void Stage::setNext(const std::string& name, const std::string& stage)
{
    next[name] = StageManager::getInstance()->doLogin(stage);
}

// ------------------------------------------------------------------------------

std::shared_ptr<StageManager> StageManager::instance(new StageManager());

std::shared_ptr<StageManager> StageManager::getInstance()
{
    return instance;    
}

void StageManager::doRegister(const std::string& name, std::shared_ptr<Stage> stage)
{
    // 名称为 name 的 Stage 没有被注册
    if (mapping.find(name) == mapping.end())
    {
        LOG_INFO << "regLster success(" << name << ")";
        mapping[name] = stage;
        return;
    }
    LOG_INFO << "register failure(" << name << ")";
}

std::shared_ptr<Stage> StageManager::doLogin(const std::string& name)
{
    // 名称为 name 的 Stage 已经被注册
    if (mapping.find(name) != mapping.end())
    {
        LOG_INFO << "login success(" << name << ")";
        return mapping[name];
    }
    LOG_INFO << "login failure(" << name << ")";
    return nullptr;
}

// ------------------------------------------------------------------------------

}   // namespace seda
}   // namespace o7si
