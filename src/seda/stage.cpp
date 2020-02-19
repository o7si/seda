#include "stage.h"

namespace o7si
{
namespace seda
{

Stage::Stage(std::string name)
    : m_name(std::move(name)), 
      m_thread_pool(&m_event_queue)
{
    m_thread_pool.setName(getShortName());
    m_performeter.setName(getShortName());
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

std::string Stage::getShortName() const
{
    // 名称中的每一个大写字母
    std::string short_name;
    for (const auto& ch : m_name)
        if (isupper(ch))
            short_name += ch;
    return short_name;    
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

size_t Stage::getPerformeterCapacity() const
{
    return m_performeter.getCapacity();
}

size_t Stage::setPerformeterCapacity(size_t capacity)
{
    m_performeter.setCapacity(capacity);
    return capacity;    
}

void Stage::performeter_internal_state() const
{
    LOG_INFO << "name = " << m_performeter.getName();
    LOG_INFO << "capacity = " << m_performeter.getCapacity();
    LOG_INFO << "longest = " << m_performeter.longest().count() << " s";
    LOG_INFO << "sum = " << m_performeter.sum().count() << " s";
    LOG_INFO << "average = " << m_performeter.average().count() << " s";
    LOG_INFO << "counter = " << m_performeter.counter();
}

void Stage::bind(EHF&& function)
{
    m_event_handler.setHandler(std::forward<EHF>(function));
}

/// 执行
void Stage::call(boost::any&& args)
{        
    // 将任务进行一层包装，将 future 的 get 方法加入到线程中执行，防止在此处阻塞
    std::function<void(boost::any&)> wrapper = [this](boost::any& args)
    {
        // 使用 packaged_task 进行一层包装
        std::packaged_task<std::pair<std::string, boost::any>(boost::any&)> packaged(m_event_handler.getHandler());
        // 获取 future 对象
        auto future = packaged.get_future();
        // 记录起始时间
        auto begin = std::chrono::system_clock::now();
        // 执行函数
        packaged(args);
        // 等待函数执行完毕，获取返回值
        auto ret = future.get();
        // 记录结束时间
        auto end = std::chrono::system_clock::now();
        // 函数执行消耗时间
        m_performeter.commit(end - begin);
        
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
