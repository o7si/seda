#include "stage.h"

namespace o7si
{
namespace seda
{

// ------------------------------------------------------------------------------

std::unordered_map<std::string, std::string> Stage::internal_state() const
{
    return {
        // 事件队列长度
        { "EventQueue.Size", std::to_string(event_queue.size()) },
        // 后续状态数目
        { "CoverMapping.Size", std::to_string(m_conver_mapping.size())}    
    };
}

Stage::Stage(std::string name, size_t max_thread)
    : m_name(name), m_max_thread(max_thread)
{
}

std::string Stage::getName() const
{
    return m_name;    
} 

void Stage::setName(std::string name)
{
    m_name = std::move(name);
}

size_t Stage::getMaxThread() const
{
    return m_max_thread;    
}

void Stage::setMaxThread(size_t max_thread)
{
    m_max_thread = max_thread;
}

void Stage::next(const std::string& state, const std::string& stage)
{
    // 状态被重复设置
    if (m_conver_mapping.find(state) != m_conver_mapping.end())    
    {
        LOG_WARN << "status set repeat: " << stage << "." << state; 
        m_conver_mapping[state] = StageManager::getInstance()->doLogin(stage);       
    }
    // 首次设置该状态
    LOG_INFO << "status set success: " << stage << "." << state;
    m_conver_mapping[state] = StageManager::getInstance()->doLogin(stage);       
}

std::shared_ptr<Stage> Stage::next(const std::string& state) 
{
    // 该状态不存在
    if (m_conver_mapping.find(state) == m_conver_mapping.end())
    {
        LOG_WARN << "status get failure: " << m_name << "." << state;
        return nullptr;
    }
    // 状态存在
    LOG_INFO << "status get success: " << m_name << "." << state;
    return m_conver_mapping[state];
}

std::unordered_map<std::string, std::shared_ptr<Stage>> Stage::next() const
{
    return m_conver_mapping;    
}

void Stage::call(const Args& args)
{
    event_queue.push(args);
}

// ------------------------------------------------------------------------------

std::shared_ptr<StageManager> StageManager::instance(new StageManager);

std::shared_ptr<StageManager> StageManager::getInstance()
{
    return instance;    
}

void StageManager::doRegister(const std::string& name, std::shared_ptr<Stage> stage)
{
    // 名称为 name 的 Stage 没有被注册
    if (mapping.find(name) == mapping.end())
    {
        // 注册成功
        LOG_INFO << "register success: " << name;
        mapping[name] = stage;
        return;
    }
    // 注册失败
    LOG_INFO << "register failure: " << name;
}

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

// ------------------------------------------------------------------------------

}   // namespace seda
}   // namespace o7si
