#pragma once

namespace o7si
{
namespace seda
{

/// 事件处理器
template<typename Function>
class EventHandler
{
public:
    /// 设置事件处理函数
    void setHandler(Function&& handler)
    {
        m_handler = std::forward<Function>(handler);    
    }
    
    /// 获取事件处理函数
    Function getHandler() const
    {
        return m_handler;
    }

private:
    /// 事件处理函数
    Function m_handler;
};
    
}   // namespace seda    
}   // namespace o7si
