#include <log.h>

void test_log_print()
{    
    // 普通日志
    LOG(DEBUG, SYSTEM) << "N";

    LOG_DEBUG(SYSTEM) << "N";
    LOG_INFO(SYSTEM) << "Y";
    LOG_WARN(SYSTEM) << "Y";
    LOG_ERROR(SYSTEM) << "Y";
    LOG_FATAL(SYSTEM) << "Y";

    LOG_DEBUG_SYS << "N";
    LOG_INFO_SYS << "Y";
    LOG_WARN_SYS << "Y";
    LOG_ERROR_SYS << "Y";
    LOG_FATAL_SYS << "Y";

    // 带检查的日志，当 check 与 target 的值相同时才输出日志
    LOG_CHECK(DEBUG, SYSTEM, 1 == 1, true) << "N";
    LOG_CHECK(DEBUG, SYSTEM, 1 == 1, false) << "N";

    LOG_CHECK_SYS(DEBUG, 1 == 1, true) << "N";
    LOG_CHECK_SYS(DEBUG, 1 == 1, false) << "N";

    LOG_DEBUG_CHECK(SYSTEM, 1 == 1, true) << "N";
    LOG_DEBUG_CHECK(SYSTEM, 1 == 1, false) << "N";
    LOG_INFO_CHECK(SYSTEM, 1 == 1, true) << "Y";
    LOG_INFO_CHECK(SYSTEM, 1 == 1, false) << "N";
    LOG_WARN_CHECK(SYSTEM, 1 == 1, true) << "Y";
    LOG_WARN_CHECK(SYSTEM, 1 == 1, false) << "N";
    LOG_ERROR_CHECK(SYSTEM, 1 == 1, true) << "Y";
    LOG_ERROR_CHECK(SYSTEM, 1 == 1, false) << "N";
    LOG_FATAL_CHECK(SYSTEM, 1 == 1, true) << "Y";
    LOG_FATAL_CHECK(SYSTEM, 1 == 1, false) << "N";

    LOG_DEBUG_CHECK_SYS(1 == 1, true) << "N";
    LOG_DEBUG_CHECK_SYS(1 == 1, false) << "N";
    LOG_INFO_CHECK_SYS(1 == 1, true) << "Y";
    LOG_INFO_CHECK_SYS(1 == 1, false) << "N";
    LOG_WARN_CHECK_SYS(1 == 1, true) << "Y";
    LOG_WARN_CHECK_SYS(1 == 1, false) << "N";
    LOG_ERROR_CHECK_SYS(1 == 1, true) << "Y";
    LOG_ERROR_CHECK_SYS(1 == 1, false) << "N";
    LOG_FATAL_CHECK_SYS(1 == 1, true) << "Y";
    LOG_FATAL_CHECK_SYS(1 == 1, false) << "N";

    LOG_SYS_CHECK(DEBUG, 1 == 1, true) << "N";

    LOG_DEBUG_SYS_CHECK(1 == 1, true) << "N";
    LOG_INFO_SYS_CHECK(1 == 1, true) << "Y";
    LOG_WARN_SYS_CHECK(1 == 1, true) << "Y";
    LOG_ERROR_SYS_CHECK(1 == 1, true) << "Y";
    LOG_FATAL_SYS_CHECK(1 == 1, true) << "Y";

    // 带检查的日志，当 check 与 target 的值相同时才输出日志
    LOG_CHECK_TRUE(DEBUG, SYSTEM, 1 == 1) << "N";

    LOG_CHECK_TRUE_SYS(DEBUG, 1 == 1) << "N";

    LOG_DEBUG_CHECK_TRUE(SYSTEM, 1 == 1) << "N";
    LOG_INFO_CHECK_TRUE(SYSTEM, 1 == 1) << "Y";
    LOG_WARN_CHECK_TRUE(SYSTEM, 1 == 1) << "Y";
    LOG_ERROR_CHECK_TRUE(SYSTEM, 1 == 1) << "Y";
    LOG_FATAL_CHECK_TRUE(SYSTEM, 1 == 1) << "Y";

    LOG_DEBUG_CHECK_TRUE_SYS(1 == 1) << "N";
    LOG_INFO_CHECK_TRUE_SYS(1 == 1) << "Y";
    LOG_WARN_CHECK_TRUE_SYS(1 == 1) << "Y";
    LOG_ERROR_CHECK_TRUE_SYS(1 == 1) << "Y";
    LOG_FATAL_CHECK_TRUE_SYS(1 == 1) << "Y";

    LOG_SYS_CHECK_TRUE(DEBUG, 1 == 1) << "N";

    LOG_DEBUG_SYS_CHECK_TRUE(1 == 1) << "N";
    LOG_INFO_SYS_CHECK_TRUE(1 == 1) << "Y";
    LOG_WARN_SYS_CHECK_TRUE(1 == 1) << "Y";
    LOG_ERROR_SYS_CHECK_TRUE(1 == 1) << "Y";
    LOG_FATAL_SYS_CHECK_TRUE(1 == 1) << "Y";

    // 带检查的日志，当 check 值为假时才输出日志
    LOG_CHECK_FALSE(DEBUG, SYSTEM, 1 == 1) << "N";

    LOG_CHECK_FALSE_SYS(DEBUG, 1 == 1) << "N";

    LOG_DEBUG_CHECK_FALSE(SYSTEM, 1 == 1) << "N";
    LOG_INFO_CHECK_FALSE(SYSTEM, 1 == 1) << "N";
    LOG_WARN_CHECK_FALSE(SYSTEM, 1 == 1) << "N";
    LOG_ERROR_CHECK_FALSE(SYSTEM, 1 == 1) << "N";
    LOG_FATAL_CHECK_FALSE(SYSTEM, 1 == 1) << "N";


    LOG_DEBUG_CHECK_FALSE_SYS(1 == 1) << "N";
    LOG_INFO_CHECK_FALSE_SYS(1 == 1) << "N";
    LOG_WARN_CHECK_FALSE_SYS(1 == 1) << "N";
    LOG_ERROR_CHECK_FALSE_SYS(1 == 1) << "N";
    LOG_FATAL_CHECK_FALSE_SYS(1 == 1) << "N";

    LOG_SYS_CHECK_FALSE(DEBUG, 1 == 1) << "N";

    LOG_DEBUG_SYS_CHECK_FALSE(1 == 1) << "N";
    LOG_INFO_SYS_CHECK_FALSE(1 == 1) << "N";
    LOG_WARN_SYS_CHECK_FALSE(1 == 1) << "N";
    LOG_ERROR_SYS_CHECK_FALSE(1 == 1) << "N";
    LOG_FATAL_SYS_CHECK_FALSE(1 == 1) << "N";
}

void test_register_behavior()
{
    // 注册行为
    // 0：
    //      用户未被注册，正常注册并返回对应指针
    //      用户已被注册，返回对应指针
    // 1：
    //      用户未被注册，正常注册并返回对应指针
    //      用户已被注册，返回空指针
    // 2：
    //      禁止注册，返回空指针
    using namespace o7si::log;
    auto lm = LoggerManager::Instance();

    lm->register_behavior(0);
    LOG_INFO_SYS << "register_behavior_0";
    LOG_INFO_SYS << lm->doRegister("rb0", Level::DEBUG); 
    LOG_INFO_SYS << lm->doRegister("rb0", Level::DEBUG); 

    lm->register_behavior(1);
    LOG_INFO_SYS << "register_behavior_1";
    LOG_INFO_SYS << lm->doRegister("rb1", Level::DEBUG); 
    LOG_INFO_SYS << lm->doRegister("rb1", Level::DEBUG); 

    lm->register_behavior(2);
    LOG_INFO_SYS << "register_behavior_2";
    LOG_INFO_SYS << lm->doRegister("rb2", Level::DEBUG); 
    LOG_INFO_SYS << lm->doRegister("rb2", Level::DEBUG); 

    lm->register_behavior(3);
    LOG_INFO_SYS << "register_behavior_3";
    LOG_INFO_SYS << lm->doRegister("rb3", Level::DEBUG); 
    LOG_INFO_SYS << lm->doRegister("rb3", Level::DEBUG); 
}

void test_login_behavior()
{
    // 登录行为
    // 0：
    //      用户已被注册，返回对应指针
    //      用户未被注册，帮助其完成注册，返回对应指针
    // 1：
    //      用户已被注册，返回对应指针
    //      用户未被注册，帮助其完成注册，并添加一个默认的控制台输出地，返回对应指针
    // 2：
    //      用户已被注册，返回对应指针
    //      用户未被注册，返回空指针
    // 3：
    //      禁止登录，返回空指针
    using namespace o7si::log;
    auto lm = LoggerManager::Instance();

    lm->login_behavior(0);
    LOG_INFO_SYS << "login_behavior_0";
    LOG_INFO_SYS << lm->doLogin("lb0");
    LOG_INFO_SYS << lm->doLogin("lb0");
    LOG_INFO(lb0) << "N";

    lm->login_behavior(1);
    LOG_INFO_SYS << "login_behavior_1";
    LOG_INFO_SYS << lm->doLogin("lb1");
    LOG_INFO_SYS << lm->doLogin("lb1");
    LOG_INFO(lb1) << "Y";

    lm->login_behavior(2);
    LOG_INFO_SYS << "login_behavior_2";
    LOG_INFO_SYS << lm->doLogin("lb2");
    LOG_INFO_SYS << lm->doLogin("lb2");
    // Segmentation fault 
    // LOG_INFO(lb2) << "N";

    lm->login_behavior(3);
    // Segmentation fault 
    // LOG_INFO_SYS << "login_behavior_3";
    // LOG_INFO_SYS << lm->doLogin("lb3");
    // LOG_INFO_SYS << lm->doLogin("lb3");
    // LOG_INFO(lb3) << "N";

    lm->login_behavior(4);
    LOG_INFO_SYS << "login_behavior_4";
    LOG_INFO_SYS << lm->doLogin("lb4");
    LOG_INFO_SYS << lm->doLogin("lb4");
    LOG_INFO(lb4) << "N";
}

int main(int argc, char* argv[])
{
    test_log_print();
    test_register_behavior();
    test_login_behavior();
    return 0;    
} 
