/******************************************************************************
 * File: api_manager.h
 * Description: 服务接口管理。
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>


// 注册接口
#define REGISTER_API(api, func) \
auto __reg_##func = [] \
{ \
    o7si::server::APIManager::Instance()->doRegister( \
        #api, o7si::server::func); \
    return nullptr; \
}();


namespace o7si
{
namespace server
{
    

// 管理服务提供的接口
class APIManager
{
public:
    // 接口函数原型定义
    // in ：入口参数（JSON）
    // out：出口参数（JSON）
    // 若程序正常执行，未出现异常，则返回值为 true
    // 若程序执行过程中出现异常，则返回值为 false
    using APIFunc = std::function<bool(const std::string& in,
                                       std::string& out)>;
public:
    // 判断是否存在对应的 API
    bool has(const std::string& api);

    // 接口的注册
    bool doRegister(const std::string& api, APIFunc&& func);

    // 调用对应的接口完成处理
    bool call(const std::string& api, 
              const std::string& in, std::string& out);

public:
    // 获取单例对象
    static std::shared_ptr<APIManager> Instance();

    // 删除拷贝构造函数，防止用户拷贝对象
    APIManager(const APIManager& other) = delete;
    
    // 删除移动构造函数，防止用户使用移动语义
    APIManager(APIManager&& other) = delete;

private:  
    // 将构造函数修饰为私有，防止用户擅自实例化对象
    APIManager() = default;

    // API 的注册表
    std::unordered_map<std::string, APIFunc> m_api_table; 
};


}   // namespace server    
}   // namespace o7si
