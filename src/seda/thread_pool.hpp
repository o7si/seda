#pragma once

#include "../pch.h"
#include "event_queue.hpp"

namespace o7si
{
namespace seda
{

class ThreadPool
{
public:
    /// 构造函数
    explicit ThreadPool(size_t capacity)
        : m_capacity(capacity), m_shutdown(false)
    {
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
        for (size_t i = 0; i < m_capacity; ++ i)
            m_threads.emplace_back(ThreadWorker(this));
    }

    /// 初始化线程池(指定数量，不能大于最大容量)
    void init(size_t number)
    {
        number = std::min(number, m_capacity);
        for (size_t i = 0; i < number; ++ i)
            m_threads.emplace_back(ThreadWorker(this));
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

    /// 析构函数
    ~ThreadPool()
    {
        destory();
    }

    template<typename Func, typename... Args>
    auto call(Func&& func, Args&&... args) -> std::future<decltype(func(args...))>
    {
        // 绑定
        std::function<decltype(func(args...))()> f = 
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
        auto task = 
            std::make_shared<std::packaged_task<decltype(func(args...))()>>(f);    
        // 函数包装器
        std::function<void()> wrapper_func = [task]
        {
            (*task)();
        };
        // 将事件添加至队列
        event_queue.push(wrapper_func);
        // 随机唤醒一个线程
        m_condition.notify_one();

        return task->get_future();
    }
private:
    /// 线程池是否被关闭
    std::atomic_bool m_shutdown;
    /// 线程池容量
    size_t m_capacity;    

    /// 事件队列
    EventQueue<std::function<void()>> event_queue;

    /// 线程容器
    std::vector<std::thread> m_threads;
    /// 互斥量
    std::mutex m_mutex;
    /// 条件变量 
    std::condition_variable m_condition;

    class ThreadWorker
    {
    public:
        ThreadWorker(ThreadPool* pool)
            : m_pool(pool)
        {
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
                    if (m_pool->event_queue.empty())
                        m_pool->m_condition.wait(ulock);
                    success = m_pool->event_queue.pop(func);
                }
                // 如果能够取出任务才执行
                // 虽然上面的代码进行过非空判断，但是在线程池销毁时会调用 notify_all 函数 
                // 所以需要再次进行判断
                if (success)
                    func();   
            }
        }
    private:    
        ThreadPool* m_pool;
    };
};

}   // namespace seda    
}   // namespace o7si
