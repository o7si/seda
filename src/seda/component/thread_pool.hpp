/******************************************************************************
 * File: thread_pool.h
 * Description: 动态线程池。 
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <atomic>
#include <thread>
#include <condition_variable>

#include "../macro.h"
#include "event_queue.hpp"
#include "performeter.hpp"


namespace o7si
{
namespace seda
{

// 线程池：并行处理任务
class ThreadPool
{
public:
    ThreadPool(EventQueue<EventQueueElem>* event_queue, 
               Performeter* performeter)
        : m_event_queue(event_queue),
          m_performeter(performeter)
    {
        m_shutdown = false;
    }
    
    std::string getName() const
    {
        return m_name;    
    }

    void setName(std::string name) 
    {
        m_name = std::move(name);
    }

    size_t getCapacity() const
    {
        return m_capacity;
    }

    void setCapacity(size_t capacity)
    {
        m_capacity = capacity;    
    }

    // 初始化线程池(最大容量)
    void init()
    {
        // 如果线程池已经被初始化
        if (m_initialize)
            return; 
        m_initialize = true;

        // 如果线程池未被初始化
        for (size_t i = 0; i < m_capacity; ++ i)
            m_threads.emplace_back(ThreadWorker(this, i));
    }

    // 初始化线程池(指定数量，不能大于最大容量)
    void init(size_t number)
    {
        // 如果线程池已经被初始化
        if (m_initialize)
            return;
        m_initialize = true;

        // 如果线程池未被初始化
        number = std::min(number, m_capacity);
        for (size_t i = 0; i < number; ++ i)
            m_threads.emplace_back(ThreadWorker(this, i));
    }

    // 销毁线程池
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

    // 唤醒一个随机线程
    void notify_one()
    {
        m_condition.notify_one();
    }

    // 唤醒所有线程
    void notify_all()
    {
        m_condition.notify_all();    
    }
    
    ~ThreadPool()
    {
        destory();
    }

private:
    // 线程池是否被关闭
    std::atomic_bool m_shutdown;
    // 线程池是否已被初始化
    std::atomic_bool m_initialize;
    
    // 线程池名称
    std::string m_name;
    // 线程池容量
    size_t m_capacity;    
    // 线程容器
    std::vector<std::thread> m_threads;
    // 互斥量
    std::mutex m_mutex;
    // 条件变量 
    std::condition_variable m_condition;

    // 事件队列
    EventQueue<EventQueueElem>* m_event_queue;
    // 性能监控器
    Performeter* m_performeter;

    // 线程工作类
    class ThreadWorker
    {
    public:
        ThreadWorker(ThreadPool* pool, size_t id)
            : m_pool(pool), m_id(id)
        {
            m_thread_name = pool->m_name + "_" + std::to_string(id);
        }
         
        void operator()()
        {
            o7si::utils::set_thread_name(m_thread_name);
            // 循环执行，直到线程池被关闭
            while (!m_pool->m_shutdown) 
            {
                EventQueueElem elem;
                bool success;
                {
                    std::unique_lock<std::mutex> ulock(m_pool->m_mutex);
                    // 如果事件队列为空，则进行等待
                    if (m_pool->m_event_queue->empty())
                        m_pool->m_condition.wait(ulock);
                    success = m_pool->m_event_queue->pop(elem);
                }
                // 如果能够取出任务才执行
                // 虽然上面的代码进行过非空判断
                // 但是在线程池销毁时会调用 notify_all 函数 
                // 所以需要再次进行判断
                if (success)
                {
                    // 纪录时间点(任务开始执行)
                    auto begin = std::chrono::system_clock::now(); 
                    // 执行任务
                    (elem.second)();   
                    // 纪录时间点(任务执行结束)
                    auto end = std::chrono::system_clock::now();

                    // wait_dura = begin - elem.first
                    // exec_dura = end - begin
                    Performeter::NoteElem note(begin - elem.first, 
                                               end - begin);
                    m_pool->m_performeter->commit(note); 
                }
            }
        }
    private:    
        ThreadPool* m_pool;
        size_t m_id;

        // 线程名称
        std::string m_thread_name;
    };
};

}   // namespace seda    
}   // namespace o7si