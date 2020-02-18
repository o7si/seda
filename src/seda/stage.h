#pragma once 

#include "../pch.h"
#include "../log.h"
#include "event_handler.hpp"
#include "event_queue.hpp"
#include "thread_pool.hpp"
#include "stage_manager.h"
#include "performeter.hpp"

namespace o7si
{
namespace seda
{

/*
 * Stage 是项目的基本组件，表示一个阶段性的任务。
 * 内部组成:
 *      1. 事件队列
 *      2. 事件处理器
 *      3. 线程池
 *      4. 性能监控器
 */

/// 基本组件
class Stage
{
public:
    /// 事件处理函数的接口
    /// pair.first  -> 处理状态
    /// pair.second -> 处理结果
    using EventHandlerFunction = std::function<std::pair<std::string, boost::any>(boost::any&)>;
    using EHF = EventHandlerFunction;

    /// 状态转换表
    using ConverMapping = std::unordered_map<std::string, std::shared_ptr<Stage>>;

public:
    /// 构造函数
    explicit Stage(std::string name);
    
    /// 为基类提供虚析构函数，保证派生类对象能够正常释放资源
    virtual ~Stage() = default;

    /// 初始化 Stage 需要使用的数据
    void init();

    /// 获取 stage 的名称
    std::string getName() const;
    
    /// 设置 stage 的名称  
    std::string setName(std::string name); 

    /// 设置 stage 的后续状态
    void next(const std::string& state, const std::string& stage);

    /// 获取 Stage 的状态转换表
    ConverMapping next() const;

    /// 根据状态名称获取 Stage 的后续状态
    std::shared_ptr<Stage> next(const std::string& state); 
    
    /// 获取线程池的最大容量
    size_t getThreadPoolCapacity() const;

    /// 设置线程池的最大容量
    size_t setThreadPoolCapacity(size_t capacity);

    /// 获取性能监控器的容量
    size_t getPerformeterCapacity() const;

    /// 设置性能监控器的容量
    size_t setPerformeterCapacity(size_t capacity);

    /// 输出性能监控器的内部信息(用于调试)
    void performeter_internal_state() const;

public:
    /// 事件处理函数
    virtual std::pair<std::string, boost::any> handler(boost::any& args) = 0;

    /// 绑定事件处理器的内部函数
    void bind(EHF&& function);

    /// 执行
    void call(boost::any&& args);

    /// 向事件队列中提交一个任务
    template<typename Function, typename... Args>
    auto commit(Function&& func, Args&&... args) -> std::future<decltype(func(args...))>
    {
        // 参数绑定
        std::function<decltype(func(args...))()> f = 
            std::bind(std::forward<Function>(func), std::forward<Args>(args)...);
        // 包装
        auto packaged = 
            std::make_shared<std::packaged_task<decltype(func(args...))()>>(f);     
        // 包装器
        std::function<void()> wrapper = [packaged]
        {
            (*packaged)();
        };
        // 将事件添加至队列
        m_event_queue.push(wrapper);
        // 随机唤醒一个线程
        m_thread_pool.notify_one();

        return packaged->get_future();
    }

protected:
    /// Stage 的名称
    std::string m_name;
    /// Stage 的状态转换表
    ConverMapping m_conver_mapping;

    /// 事件处理状态
    std::string state;

    /// 事件队列
    EventQueue<std::function<void()>> m_event_queue;
    /// 事件处理器
    EventHandler<EHF> m_event_handler;
    /// 线程池
    ThreadPool m_thread_pool;
    /// 性能监控器
    Performeter m_performeter;
};

}   // namespace seda
}   // namespace o7si
