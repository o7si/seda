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
    // 将 HTTP 请求的资源返回给客户端
    std::string content;
    bool load_ret = o7si::utils::load_file(m_root + request->path(), 
                                           content); 
    response->body(content);
}

// ----------------------------------------------------------------------------


}   // namespace net    
}   // namespace o7si
