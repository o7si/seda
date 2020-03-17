#include "stage.h"

namespace o7si
{
namespace seda
{

Stage::Stage(std::string name)
    : m_name(std::move(name)), 
      m_run(false),
      m_thread_pool(&m_event_queue, &m_performeter)
{
    // 生成 Stage 的短名称
    std::string short_name;
    for (const auto& ch : m_name)
        if (isupper(ch) || isdigit(ch))
            short_name += ch;
    m_short_name = short_name;

    m_thread_pool.setName(getShortName());
    m_performeter.setName(getShortName());
}

bool Stage::setName(std::string new_name)
{
    bool ret = StageManager::Instance()->doUpdate(m_name, new_name);    
    if (!ret)
        return false;
    m_name = std::move(new_name);    
    return true;
}

void Stage::next(const std::string& state, const std::string& stage)
{
    // 该状态已经被设置过
    if (m_conver_mapping.find(state) != m_conver_mapping.end())
    {
        m_conver_mapping[state] = StageManager::Instance()->doLogin(stage); 
        LOG_WARN_SYS << m_name << " set state: " << state << " -> " << stage 
                     << "(repeat)";
    }
    // 首次设置该状态
    m_conver_mapping[state] = StageManager::Instance()->doLogin(stage);
    LOG_INFO_SYS << m_name << " set state: " << state << " -> " << stage 
                 << "(first)";
}

ConverMapping Stage::next() const
{
    return m_conver_mapping;    
}

std::shared_ptr<Stage> Stage::next(const std::string& state)
{
    if (m_conver_mapping.find(state) == m_conver_mapping.end())
        return nullptr;
    return m_conver_mapping[state];
}

void Stage::bind(EventHandlerFunc&& function)
{
    m_event_handler.setHandler(std::forward<EventHandlerFunc>(function));
}

void Stage::call(boost::any&& args)
{        
    // 进行一层包装，将 future 的 get 方法加入到线程中执行，防止在此处阻塞
    std::function<void(boost::any&)> wrapper = [this](boost::any& args)
    {
        // 使用 packaged_task 进行一层包装
        std::packaged_task<std::pair<std::string, boost::any>(boost::any&)> 
            packaged(m_event_handler.getHandler());
        // 获取 future 对象
        auto future = packaged.get_future();
        // 执行函数
        packaged(args);
        // 等待函数执行完毕，获取返回值
        auto ret = future.get();
        
        // 根据返回值决定后续状态
        // 如果不存在后续状态
        if (m_conver_mapping.find(ret.first) == m_conver_mapping.end())
            return;
        // 如果存在后续状态
        m_conver_mapping[ret.first]->call(
            std::forward<boost::any>(ret.second));
    };
    // 提交任务
    commit(std::move(wrapper), std::forward<boost::any>(args));
}

}   // namespace seda    
}   // namespace o7si
