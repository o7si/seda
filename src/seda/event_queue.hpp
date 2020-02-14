#pragma once

#include "../pch.h"

namespace o7si
{
namespace seda
{

/// 事件队列
template<typename Type>
class EventQueue
{
public:
    /// 查看队列首部的元素但不移除
    Type peek()
    {
        std::unique_lock<std::mutex> ulock(m_mutex);
        return m_container.front();
    }

    /// 查看队列首部的元素并且移除
    Type pop()
    {
        std::unique_lock<std::mutex> ulock(m_mutex);
        Type elem = m_container.front(); 
        m_container.pop();
        ulock.unlock();
        return elem;
    }

    /// 向队列尾部添加元素
    void push(const Type& elem)
    {
        std::unique_lock<std::mutex> ulock(m_mutex);
        m_container.push(elem);   
    }

    /// 查看队列元素个数
    size_t size() const
    {
        std::unique_lock<std::mutex> ulock(m_mutex);
        size_t ret = m_container.size();
        ulock.unlock();
        return ret;
    }

    /// 查看队列是否为空
    bool empty() const
    {
        std::unique_lock<std::mutex> ulock(m_mutex);
        bool ret = m_container.empty();
        ulock.unlock();
        return ret;
    }

private:
    /// 内部容器
    std::queue<Type> m_container;    
    /// 互斥量
    mutable std::mutex m_mutex;
};    

}   // namespace seda    
}   // namespace o7si
