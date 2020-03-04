/******************************************************************************
 * File: stage.h
 * Description: 表示一个阶段性的任务，是项目的核心组件。
 * Author: o7si
 *****************************************************************************/
#pragma once 


#include <future>
#include <functional>

#include <boost/any.hpp>

#include "../log.h"
#include "macro.h"
#include "stage_manager.h"
#include "component/event_handler.hpp"
#include "component/event_queue.hpp"
#include "component/performeter.hpp"
#include "component/thread_pool.hpp"

namespace o7si
{
namespace seda
{

// Stage 是项目的核心组件，表示一个阶段性的任务。
// 内部组成:
// 1. 事件队列
// 2. 事件处理器
// 3. 线程池
// 4. 性能监控器

class Stage
{
public:
    explicit Stage(std::string name);
    
    virtual ~Stage() = default;

    void init();

    std::string getName() const
    {
        return m_name;    
    }

    void setName(std::string name)
    {
        m_name = std::move(name);    
    } 

    std::string getShortName() const
    {
        return m_short_name;    
    }

    void setShortName(std::string short_name)
    {
        m_short_name = std::move(short_name);   
    }

    // 获取线程池的最大容量
    size_t getThreadPoolCapacity() const
    {
        return m_thread_pool.getCapacity();
    }

    // 设置线程池的最大容量
    void setThreadPoolCapacity(size_t capacity)
    {
        m_thread_pool.setCapacity(capacity);   
    }

    // 获取性能监控器的容量
    size_t getPerformeterCapacity() const
    {
        return m_performeter.getCapacity();    
    }

    // 设置性能监控器的容量
    void setPerformeterCapacity(size_t capacity)
    {
        m_performeter.setCapacity(capacity);    
    }

    // 设置 Stage 的后续状态
    void next(const std::string& state, const std::string& stage);

    // 获取 Stage 的状态转换表
    ConverMapping next() const;

    // 根据状态名称获取 Stage 的后续状态
    std::shared_ptr<Stage> next(const std::string& state); 
    
public:
    // 输出性能监控器的内部信息(用于调试)
    void performeter_internal_state() const;

public:
    // 事件处理函数
    virtual std::pair<std::string, boost::any> handler(boost::any& args) = 0;

    // 绑定事件处理器的内部函数
    void bind(EventHandlerFunc&& function);

    // 执行
    void call(boost::any&& args);

    // 向事件队列中提交一个任务
    template<typename Function, typename... Args>
    auto commit(Function&& func, Args&&... args) 
        -> std::future<decltype(func(args...))>
    {
        // 参数绑定
        std::function<decltype(func(args...))()> f = 
            std::bind(std::forward<Function>(func), 
                      std::forward<Args>(args)...);
        // 包装
        auto packaged = 
            std::make_shared<std::packaged_task<decltype(func(args...))()>>(f);     
        // 包装器
        std::function<void()> wrapper = [packaged]
        {
            (*packaged)();
        };
        // 将事件添加至队列
        m_event_queue.push(
            std::make_pair(std::chrono::system_clock::now(), wrapper));
        // 随机唤醒一个线程
        m_thread_pool.notify_one();

        return packaged->get_future();
    }

protected:
    // 名称
    std::string m_name;
    // 短名称
    std::string m_short_name;
    // 状态转换表
    ConverMapping m_conver_mapping;

    // 事件处理状态
    std::string state;

    // 事件队列
    EventQueue<EventQueueElem> m_event_queue;
    // 事件处理器
    EventHandler<EventHandlerFunc> m_event_handler;
    // 线程池
    ThreadPool m_thread_pool;
    // 性能监控器
    Performeter m_performeter;
};

}   // namespace seda
}   // namespace o7si
