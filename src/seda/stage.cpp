#include "stage.h"

namespace o7si
{
namespace seda
{

Stage::Stage(std::string name)
    : m_name(std::move(name)), 
      m_thread_pool(&m_event_queue)
{
    m_thread_pool.setName(m_name + "_ThreadPool");
}

void Stage::init()
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

size_t Stage::getThreadPoolCapacity() const
{
    return m_thread_pool.getCapacity();
}

size_t Stage::setThreadPoolCapacity(size_t capacity)
{
    m_thread_pool.setCapacity(capacity);
    return capacity;    
}

void Stage::bind(EHF&& function)
{
    m_event_handler.setHandler(std::forward<EHF>(function));
}

/// 执行
void Stage::call(boost::any&& args)
{
    // 提交事件
    auto future = commit(
        m_event_handler.getHandler(),
        std::forward<boost::any>(args)
    );

    auto ret = future.get();

    auto next = m_conver_mapping[ret.first];
    // 如果不存在后续状态
    if (next == nullptr)
        return;
    // 如果存在后续状态
    next->call(std::forward<boost::any>(ret.second));
}

}   // namespace seda    
}   // namespace o7si
