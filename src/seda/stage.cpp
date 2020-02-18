#include "stage.h"

namespace o7si
{
namespace seda
{

Stage::Stage(std::string name, size_t capacity)
    : m_name(std::move(name)), 
      m_thread_pool(&m_event_queue, capacity)
{
    // 初始化线程池
    m_thread_pool.init();
}

std::string Stage::getName() const
{
    return m_name;
}

std::string Stage::setName(std::string name) 
{
    m_name = std::move(name);
    return m_name;    
}

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

Stage::ConverMapping Stage::next() const
{
    return m_conver_mapping;    
}

std::shared_ptr<Stage> Stage::next(const std::string& state)
{
    return m_conver_mapping[state];
}

void Stage::bind(EHF&& function)
{
    m_event_handler.setHandler(std::forward<EHF>(function));
}

/// 执行
void Stage::call(boost::any&& args)
{
    auto future = commit(
        m_event_handler.getHandler(),
        std::forward<boost::any>(args)
    );
    auto ret = future.get();
    LOG_DEBUG << ret.first << " " << boost::any_cast<std::string>(ret.second);
}

}   // namespace seda    
}   // namespace o7si
