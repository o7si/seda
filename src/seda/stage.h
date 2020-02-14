#pragma once 

#include "../pch.h"
#include "../log.h"


#include "event_queue.hpp"

#define REGISTER_STAGE(stage) \
auto __##stage = [] \
{ \
    NAMESPACE_SEDA::StageManager::getInstance()->doRegister( \
        #stage, \
        std::make_shared<NAMESPACE_SEDA::stage>(#stage, 0)); \
    return nullptr; \
}();

namespace o7si
{
namespace seda
{
/// Stage 的重要组成
/// 1. 事件队列:
/// 2. 事件处理器:
/// 3. 动态线程池: 
/// 4. 性能监控器: 根据实际运行情况调整 Stage 的资源占用情况。
class Stage
{
    using Args = std::unordered_map<std::string, boost::any>;
public:
    /// 返回 Stage 的内部状态(不推荐使用，一般用于调试)
    std::unordered_map<std::string, std::string> internal_state() const;

    Stage(std::string name, size_t max_thread);

    std::string getName() const;
    void setName(std::string name);

    size_t getMaxThread() const;
    void setMaxThread(size_t max_thread);

    /// 设置 Stage 的后续状态
    void next(const std::string& state, const std::string& stage);
    /// 根据状态名称获取后续 Stage
    std::shared_ptr<Stage> next(const std::string& state);
    /// 获取整个状态转换表(不推荐使用，一般用于调试)
    std::unordered_map<std::string, std::shared_ptr<Stage>> next() const;   
    
    /// 任务处理入口
    void call(const Args& args);

    /// 为纯虚基类提供析构函数，使得派生类资源能够正常释放
    virtual ~Stage() = default;

protected:    
    /// 名称
    std::string m_name;
    /// 最大允许线程数
    size_t m_max_thread;
    
    /// 状态转换表
    std::unordered_map<std::string, std::shared_ptr<Stage>> m_conver_mapping;
    /// 事件队列
    EventQueue<Args> event_queue;
    /// 事件处理器
    /// 动态线程池
    // ThreadPool m_thread_pool;
    /// 性能监控器
    // Performeter m_performeter;
};

/// Stage 的管理类(单例模式)
class StageManager
{
public:
    /// 注册 Stage 到管理类
    void doRegister(const std::string& name, std::shared_ptr<Stage> stage);
    
    /// 从管理类中获取 Stage 
    std::shared_ptr<Stage> doLogin(const std::string& name);

    /// 获取 Stage 的注册映射表(不推荐使用，一般用于调试)
    std::unordered_map<std::string, std::shared_ptr<Stage>> registerMapping() const;

public:
    /// 获取 StageManager 的单例对象
    static std::shared_ptr<StageManager> getInstance();
    
    /// 删除拷贝构造函数，防止用户拷贝该对象
    StageManager(const StageManager& other) = delete;

private:
    /// 将构造函数修饰为私有，防止用户擅自实例化对象
    StageManager() = default;

    /// 字符串和 Stage 的映射关系
    std::unordered_map<std::string, std::shared_ptr<Stage>> mapping;
    /// 单例对象
    static std::shared_ptr<StageManager> instance;
};
    
}   // namespace seda
}   // namespace o7si
