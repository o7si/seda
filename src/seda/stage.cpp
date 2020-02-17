#include "stage.h"

namespace o7si
{
namespace seda
{

/// 构造函数
Stage::Stage(std::string name, size_t capacity = 1)
    : m_name(std::move(name)), m_thread_pool(capacity)
{
    // 初始化线程池
    m_thread_pool.init();
}

/// 获取 stage 的名称
std::string Stage::getName() const
{
    return m_name;
}

/// 设置 stage 的名称  
std::string Stage::setName(std::string name) 
{
    m_name = std::move(name);
    return m_name;    
}

/// 设置 stage 的后续状态
void Stage::next(const std::string& state, const std::string& stage)
{
    // 该状态已经被设置过
    if (m_conver_mapping.find(state) != m_conver_mapping.end())
    {
        m_conver_mapping[state] = StageManager::getInstance()->doLogin(stage); 
        LOG_INFO << m_name << " set state: " << state << " -> " << stage << "(repeat)";
    }
    // 首次设置该状态
    m_conver_mapping[state] = StageManager::getInstance()->doLogin(stage);
    LOG_INFO << m_name << " set state: " << state << " -> " << stage << "(first)";
}

/// 绑定事件处理器的内部函数
void Stage::bind(EHFunction&& function)
{
    m_event_handler.setHandler(std::forward<EHFunction>(function));
}

/// 执行
void Stage::call(boost::any&& args)
{
    std::future<boost::any> future = m_thread_pool.call(
            m_event_handler.getHandler(), 15 
            //std::bind(m_event_handler.gethandler(), std::forward<boost::any>(args))
            );

    LOG_DEBUG << boost::any_cast<std::string>(future.get());
}

}   // namespace seda    
}   // namespace o7si
