#pragma once 

#include "../pch.h"
#include "../log.h"
#include "event_handler.hpp"
#include "event_queue.hpp"
#include "thread_pool.hpp"
#include "stage_manager.h"

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
    using EventHandlerFunction = std::function<boost::any(boost::any&)>;
    using EHF = EventHandlerFunction;

    /// 状态转换表
    using ConverMapping = std::unordered_map<std::string, std::shared_ptr<Stage>>;

public:
    /// 构造函数
    Stage(std::string name, size_t capacity);
    
    /// 为基类提供虚析构函数，保证派生类对象能够正常释放资源
    virtual ~Stage() = default;

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
    
public:
    /// 绑定事件处理器的内部函数
    void bind(EHF&& function);

    /// 执行
    void call(boost::any&& args);

    /// 事件处理函数
    virtual boost::any handler(boost::any& args) = 0;

protected:
    /// Stage 的名称
    std::string m_name;
    /// Stage 的状态转换表
    ConverMapping m_conver_mapping;

    /// 事件处理状态
    std::string state;

    /// 事件队列
    /// EventQueue m_event_queue;
    /// 事件处理器
    EventHandler<EHF> m_event_handler;
    /// 线程池
    ThreadPool m_thread_pool;
    /// 性能监控器
    /// Performeter m_performeter;
};

}   // namespace seda
}   // namespace o7si
