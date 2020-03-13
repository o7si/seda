#include "servlet.h"


namespace o7si
{
namespace net
{


// ----------------------------------------------------------------------------

Servlet::Servlet(std::string root)
    : m_root(std::move(root))
{
}
    
void Servlet::todo(std::shared_ptr<HttpRequest> request,
                     std::shared_ptr<HttpResponse> response)
{
    // 判断 HTTP 请求的资源类型
    
    // 将 HTTP 请求的资源返回给客户端
    std::string content;
    bool load_ret = o7si::utils::load_file(m_root + request->path(), 
                                           content); 
    std::cout << request->format() << std::endl;

    if (!load_ret)
        content = R"+({"username": "o7si", "password": "123456"})+";
    response->status(o7si::net::GenStatusFrom(200));
    response->body(content);

    LOG_DEBUG_SYS << "\n" << request->format();
    // LOG_DEBUG_SYS << "\n" << response->format();
}

// ----------------------------------------------------------------------------


}   // namespace net    
}   // namespace o7si
