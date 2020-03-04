/******************************************************************************
 * File: performeter.h
 * Description: 性能监控器。 
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <algorithm>
#include <chrono>

#include "../macro.h"


namespace o7si
{
namespace seda
{
    
// 性能监控器：记录各项指标，默认容量为 100
class Performeter
{
public:
    // 耗时(类型)
    using Duration = std::chrono::duration<double>;

public:
    // 纪录的内容
    class NoteElem
    {
    public:
        NoteElem(const Duration& wait_dura, const Duration& exec_dura)
            : m_wait_dura(wait_dura), m_exec_dura(exec_dura)
        {
        }
        
        // 获取线程等待耗时
        Duration wait_dura() const
        {
            return m_wait_dura;    
        }

        // 设置线程等待耗时
        void wait_dura(const Duration& dura)
        {
            m_wait_dura = dura;    
        }

        // 获取任务执行耗时
        Duration exec_dura() const
        {
            return m_exec_dura;    
        }

        // 设置任务执行耗时
        void exec_dura(const Duration& dura)
        {
            m_exec_dura = dura;    
        }

        // 设置描述信息
        void setDesc(std::string desc)
        {
            m_desc = std::move(desc);
        }

        // 获取描述信息
        std::string getDesc() const
        {
            return m_desc;    
        }

    private:     
        // 线程等待耗时
        Duration m_wait_dura;
        // 任务执行耗时
        Duration m_exec_dura;
        // 额外的描述
        std::string m_desc;  
    };

public:
    Performeter(size_t capacity = 100)
        : m_capacity(capacity)
    {    
        // 容量最小为 1
        m_capacity = std::max(static_cast<size_t>(1), m_capacity);
    } 

    // 获取监控器的名称
    std::string getName() const
    {
        return m_name;    
    }

    // 设置监控器的名称
    void setName(std::string name)
    {
        std::lock_guard<std::mutex> ulock(m_mutex);
        m_name = std::move(name);
    }

    // 修改监控器的容量
    void setCapacity(size_t capacity)
    {
        std::lock_guard<std::mutex> glock(m_mutex);
        m_capacity = capacity;
        m_capacity = std::max(static_cast<size_t>(1), m_capacity);
    }

    // 获取监控器的容量
    size_t getCapacity() const
    {
        std::lock_guard<std::mutex> glock(m_mutex);
        return m_capacity;    
    }

    // 获取最长耗时(线程等待 + 任务执行)
    Duration longest_dura() const
    {
        return m_longest_wait_dura + m_longest_exec_dura;    
    }

    // 获取最长耗时(线程等待)
    Duration longest_wait_dura() const
    {
        return m_longest_wait_dura;    
    }

    // 获取最长耗时(任务执行耗时)
    Duration longest_exec_dura() const
    {
        return m_longest_exec_dura;    
    }

    // 获取总耗时(线程等待 + 任务执行)
    Duration sum_dura() const
    {
        return m_sum_wait_dura + m_sum_exec_dura;    
    }

    // 获取总耗时(线程等待)
    Duration sum_wait_dura() const
    {
        return m_sum_wait_dura;    
    }

    // 获取总耗时(任务执行)
    Duration sum_exec_dura() const
    {
        return m_sum_exec_dura;    
    }

    // 获取平均耗时(线程等待 + 任务执行)
    Duration avg_dura() const
    {
        if (m_counter == 0)
            return Duration(0);
        return (m_sum_wait_dura + m_sum_exec_dura) / m_counter;    
    }

    // 获取平均耗时(线程等待)
    Duration avg_wait_dura() const
    {
        if (m_counter == 0)
            return Duration(0);
        return m_sum_wait_dura / m_counter;    
    } 

    // 获取平均耗时(任务执行)
    Duration avg_exec_dura() const
    {
        if (m_counter == 0)
            return Duration(0);
        return m_sum_exec_dura / m_counter;    
    }

    // 获取总执行次数
    size_t count() const
    {
        return m_counter;    
    }

    // 提交一次纪录
    void commit(const NoteElem& elem)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        // 如果已经到达容量上限，则删除第一个元素
        if (m_notes.size() >= m_capacity)
            m_notes.erase(m_notes.begin());

        // 更新相关的成员变量
        m_longest_wait_dura = std::max(m_longest_wait_dura, elem.wait_dura());
        m_longest_exec_dura = std::max(m_longest_exec_dura, elem.exec_dura());
        m_sum_wait_dura += elem.wait_dura();
        m_sum_exec_dura += elem.exec_dura();
        ++ m_counter;
        
        m_notes.push_back(elem); 
    }

    void commit(const Duration& wait_dura, const Duration& exec_dura)
    {
        commit(NoteElem(wait_dura, exec_dura));   
    }

private:
    // 监控器名称
    std::string m_name;
    // 监控器容量 
    size_t m_capacity;

    // 纪录
    std::list<NoteElem> m_notes; 
    
    // 最长耗时(线程等待)
    Duration m_longest_wait_dura;
    // 最长耗时(任务执行)
    Duration m_longest_exec_dura;
    // 总耗时(线程等待)
    Duration m_sum_wait_dura;
    // 总耗时(线程执行)
    Duration m_sum_exec_dura;
    // 总执行次数
    size_t m_counter;

    mutable std::mutex m_mutex;

};

}   // namespace seda   
}   // namespace o7si
