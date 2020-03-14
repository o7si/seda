#include "api_manager.h"


namespace o7si
{
namespace server
{


std::shared_ptr<APIManager> APIManager::Instance()
{
    static std::shared_ptr<APIManager> instance(new APIManager());
    return instance;    
}

bool APIManager::has(const std::string& api)
{
    return m_api_table.find(api) != m_api_table.end();    
}

bool APIManager::doRegister(const std::string& api, APIFunc&& func)
{
    // 无法存在同名称的接口，当接口已经存在时，无法完成注册
    if (has(api))
        return false;

    m_api_table[api] = std::forward<APIFunc>(func);
    return true;
}

bool APIManager::call(const std::string& api, 
                      const std::string& in, std::string& out)
{
    // 如果 API 不存在
    if (!has(api)) 
    {
        out = ""; 
        return false;    
    }
    return m_api_table[api](in, out);
}


}   // namespace server    
}   // namespace o7si
