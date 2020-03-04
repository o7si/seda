#include "stage.h"

namespace o7si
{
namespace seda
{

Stage::Stage(std::string name)
    : m_name(std::move(name)), 
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

void Stage::init()
{
    // 初始化线程池
    m_thread_pool.init();    
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

void Stage::performeter_internal_state() const
{
    // 调试性代码
    LOG_INFO_SYS << "name = " << m_performeter.getName();
    LOG_INFO_SYS << "capacity = " << m_performeter.getCapacity();

    LOG_INFO_SYS << "longest = " << m_performeter.longest_dura().count() << " s";
    LOG_INFO_SYS << "longest(wait) = " << m_performeter.longest_wait_dura().count() << " s";
    LOG_INFO_SYS << "longest(exec) = " << m_performeter.longest_exec_dura().count() << " s";

    LOG_INFO_SYS << "sum = " << m_performeter.sum_dura().count() << " s";
    LOG_INFO_SYS << "sum(wait) = " << m_performeter.sum_wait_dura().count() << " s";
    LOG_INFO_SYS << "sum(exec) = " << m_performeter.sum_exec_dura().count() << " s";

    LOG_INFO_SYS << "average = " << m_performeter.avg_dura().count() << " s";
    LOG_INFO_SYS << "average(wait) = " << m_performeter.avg_wait_dura().count() << " s";
    LOG_INFO_SYS << "average(exec) = " << m_performeter.avg_exec_dura().count() << " s";

    LOG_INFO_SYS << "counter = " << m_performeter.count();
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
        auto next = m_conver_mapping[ret.first];
        // 如果不存在后续状态
        if (next == nullptr)
            return;
        // 如果存在后续状态
        next->call(std::forward<boost::any>(ret.second));
    };
    // 提交任务
    commit(std::move(wrapper), std::forward<boost::any>(args));
}

}   // namespace seda    
}   // namespace o7si
