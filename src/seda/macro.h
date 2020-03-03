#pragma once

/// 注册并绑定函数(Stage)
#define REGISTER_STAGE(stage) \
auto __reg_##stage = [] \
{ \
    auto reg_stage = o7si::seda::StageManager::getInstance()->doRegister( \
        #stage, std::make_shared<o7si::seda::stage>(#stage) \
    ); \
    reg_stage->bind([reg_stage](boost::any& args) \
    { \
        return reg_stage->handler(args); \
    }); \
    return reg_stage; \
}();

/// 登录(Stage)
#define LOGIN_STAGE(name) o7si::seda::StageManager::getInstance()->doLogin(#name);

/// 状态转换表
#define ConverMapping std::unordered_map<std::string, std::shared_ptr<Stage>>

/// 事件队列元素类型
/// pair.first  -> 进入队列的时间点
/// pair.second -> 任务
#define EventQueueElem std::pair<std::chrono::time_point<std::chrono::system_clock>, std::function<void()>>   

/// 事件处理函数的接口
/// pair.first  -> 处理状态
/// pair.second -> 处理结果
#define EventHandlerFunc std::function<std::pair<std::string, boost::any>(boost::any&)>
