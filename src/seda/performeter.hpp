#pragma once

#include "../pch.h"

namespace o7si
{
namespace seda
{
    
/// 性能监控器(默认容量为 100)
class Performeter
{
public:
    /// 时间类型
    using Timer = std::chrono::duration<double>;

public:
    /// 构造函数
    Performeter(size_t capacity = 100)
        : m_capacity(capacity)
    {    
    } 

    /// 设置监控器的名称
    std::string setName(std::string name)
    {
        std::lock_guard<std::mutex> ulock(m_mutex);
        m_name = std::move(name);
        return m_name;    
    }

    /// 获取监控器的名称
    std::string getName() const
    {
        std::lock_guard<std::mutex> ulock(m_mutex);
        return m_name;    
    }

    /// 修改监控器的容量
    size_t setCapacity(size_t capacity)
    {
        std::lock_guard<std::mutex> glock(m_mutex);
        m_capacity = capacity;
        return m_capacity;    
    }

    /// 获取监控器的容量
    size_t getCapacity() const
    {
        std::lock_guard<std::mutex> glock(m_mutex);
        return m_capacity;    
    }

    /// 获取最长的执行耗时
    Timer longest() const
    {
        std::lock_guard<std::mutex> glock(m_mutex);
        return m_longest_timer;    
    }

    /// 获取总耗时
    Timer sum() const
    {
        std::lock_guard<std::mutex> glock(m_mutex);
        return m_sum_timer;    
    }

    /// 获取平均耗时
    Timer average() const
    {
        std::lock_guard<std::mutex> glock(m_mutex);
        if (m_counter == 0)
            return Timer(0);
        return m_sum_timer / m_counter;    
    }

    /// 获取总执行次数
    size_t counter() const
    {
        std::lock_guard<std::mutex> glock(m_mutex);
        return m_counter;    
    }

    /// 获取执行列表
    std::list<Timer> notes() const
    {
        std::lock_guard<std::mutex> glock(m_mutex);
        return m_notes;    
    }
    
    /// 提交一次纪录
    void commit(const Timer& note)
    {
        std::lock_guard<std::mutex> glock(m_mutex);
        // 如果已经达到容量上限，则删除第一个元素
        if (m_notes.size() >= m_capacity)
            m_notes.erase(m_notes.begin());
        m_notes.push_back(note);

        // 更新相关值
        m_longest_timer = std::max(m_longest_timer, note);
        m_sum_timer += note;
        ++ m_counter; 
    }
private:
    /// 监控器名称
    std::string m_name;
    /// 监控器容量 
    size_t m_capacity;
    /// 每一次执行的耗时
    std::list<Timer> m_notes;
    
    /// 最长耗时
    Timer m_longest_timer;
    /// 总耗时
    Timer m_sum_timer;
    /// 执行次数
    size_t m_counter;

    /// 互斥量
    mutable std::mutex m_mutex;
};

}   // namespace seda   
}   // namespace o7si
