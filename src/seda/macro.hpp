#pragma once

/// 注册并绑定函数(Stage)
#define REGISTER_STAGE(stage) \
auto __reg_##stage = [] \
{ \
    auto reg_stage = NAMESPACE_SEDA::StageManager::getInstance()->doRegister( \
        #stage, std::make_shared<NAMESPACE_SEDA::stage>(#stage) \
    ); \
    reg_stage->bind([reg_stage](boost::any& args) \
    { \
        return reg_stage->handler(args); \
    }); \
    return reg_stage; \
}();

/// 登录(Stage)
#define LOGIN_STAGE(name) NAMESPACE_SEDA::StageManager::getInstance()->doLogin(#name);

/// Stage 内部模版成员的类型定义
class TypeDef
{
public:
    /// 事件队列的元素类型
    using EventQueueElem = std::pair<std::chrono::time_point<std::chrono::system_clock>, std::function<void()>>;   
};
