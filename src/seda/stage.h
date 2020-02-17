#pragma once 

#include "../pch.h"
#include "../log.h"
#include "event_handler.hpp"
#include "event_queue.hpp"
#include "thread_pool.hpp"
#include "stage_manager.h"

#define REGISTER_STAGE(stage) \
auto __##stage = [] \
{ \
    auto reg_stage = NAMESPACE_SEDA::StageManager::getInstance()->doRegister( \
        #stage, std::make_shared<NAMESPACE_SEDA::stage>(#stage, 1) \
    ); \
    return nullptr; \
}();

namespace o7si
{
namespace seda
{

/// 基础组件: 表示一个阶段
/// 内部组成:
/// 1. 事件队列  : EventQueue
/// 2. 事件处理器: EventHandler
/// 3. 线程池    : ThreadPool
/// 4. 性能监控器: Performeter  
class Stage
{
public:
    /// EventHandlerFunction
    using EHFunction = std::function<boost::any(boost::any&&)>;

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

public:
    /// 绑定事件处理器的内部函数
    void bind(EHFunction&& function);

    /// 执行
    void call(boost::any&& args);

    /// 事件处理函数
    virtual boost::any handler(boost::any&& args) = 0;

protected:
    /// Stage 的名称
    std::string m_name;
    /// Stage 的状态转换表
    std::unordered_map<std::string, std::shared_ptr<Stage>> m_conver_mapping;

    /// 事件队列
    /// EventQueue m_event_queue;
    /// 事件处理器
    EventHandler<EHFunction> m_event_handler;
    /// 线程池
    ThreadPool m_thread_pool;
    /// 性能监控器
    /// Performeter m_performeter;
};

}   // namespace seda
}   // namespace o7si
