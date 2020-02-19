#pragma once

#include "../pch.h"
#include "event_queue.hpp"

namespace o7si
{
namespace seda
{

/// 线程池
class ThreadPool
{
public:
    /// 构造函数
    explicit ThreadPool(EventQueue<std::function<void()>>* event_queue)
        : m_event_queue(event_queue)
    {
        m_shutdown = false;
    }
    
    /// 获取线程池名称
    std::string getName() const
    {
        return m_name;    
    }

    /// 设置线程池名称
    std::string setName(std::string name) 
    {
        m_name = std::move(name);
        return m_name;    
    }

    /// 设置线程池最大容量
    void setCapacity(size_t capacity)
    {
        m_capacity = capacity;    
    }

    /// 获取线程池最大容量
    size_t getCapacity() const
    {
        return m_capacity;
    }

    /// 初始化线程池(最大容量)
    void init()
    {
        // 如果线程池已经被初始化
        if (m_initialize)
            return; 
        // 如果线程池未被初始化
        for (size_t i = 0; i < m_capacity; ++ i)
            m_threads.emplace_back(ThreadWorker(this, i));
    }

    /// 初始化线程池(指定数量，不能大于最大容量)
    void init(size_t number)
    {
        // 如果线程池已经被初始化
        if (m_initialize)
            return;
        // 如果线程池未被初始化
        number = std::min(number, m_capacity);
        for (size_t i = 0; i < number; ++ i)
            m_threads.emplace_back(ThreadWorker(this, i));
    }

    /// 销毁线程池
    void destory()
    {
        // 设置线程池为关闭状态
        m_shutdown = true;
        // 唤醒所有正在等待的线程
        m_condition.notify_all(); 
        // 等待所有线程执行完毕
        for (auto iter = m_threads.begin(); iter != m_threads.end(); ++ iter)
        {
            if (iter->joinable())
                iter->join();
        }
    }

    /// 唤醒一个随机线程
    void notify_one()
    {
        m_condition.notify_one();
    }

    /// 唤醒所有线程
    void notify_all()
    {
        m_condition.notify_all();    
    }
    
    /// 析构函数
    ~ThreadPool()
    {
        destory();
    }

private:
    /// 线程池是否被关闭
    std::atomic_bool m_shutdown;
    /// 线程池是否已被初始化
    std::atomic_bool m_initialize;
    
    /// 线程池名称
    std::string m_name;
    /// 线程池容量
    size_t m_capacity;    
    /// 线程容器
    std::vector<std::thread> m_threads;
    /// 互斥量
    std::mutex m_mutex;
    /// 条件变量 
    std::condition_variable m_condition;

    /// 事件队列
    EventQueue<std::function<void()>>* m_event_queue;

    /// 线程工作类
    class ThreadWorker
    {
    public:
        /// 构造函数
        ThreadWorker(ThreadPool* pool, size_t id)
            : m_pool(pool), m_id(id)
        {
            o7si::utils::set_thread_name(pool->m_name + "_" + std::to_string(id));
        }
         
        void operator()()
        {
            // 循环执行，直到线程池被关闭
            while (!m_pool->m_shutdown) 
            {
                std::function<void()> func;
                bool success;
                {
                    std::unique_lock<std::mutex> ulock(m_pool->m_mutex);
                    // 如果事件队列为空，则进行等待
                    if (m_pool->m_event_queue->empty())
                        m_pool->m_condition.wait(ulock);
                    success = m_pool->m_event_queue->pop(func);
                }
                // 如果能够取出任务才执行
                // 虽然上面的代码进行过非空判断，但是在线程池销毁时会调用 notify_all 函数 
                // 所以需要再次进行判断
                if (success)
                {
                    LOG_DEBUG << m_pool->m_name << "." << m_id << " call function";
                    func();   
                }
            }
        }
    private:    
        ThreadPool* m_pool;
        size_t m_id;
    };
};

}   // namespace seda    
}   // namespace o7si
