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
    // HTTP Method == GET
    if (request->method() == o7si::net::HttpMethod::HTTP_METHOD_GET)
    {
        doGet(request, response);    
        return;
    }
    // HTTP Method == POST 
    if (request->method() == o7si::net::HttpMethod::HTTP_METHOD_POST)
    {
        doPost(request, response); 
        return;    
    }
    // 无法处理的情况
    response->status(o7si::net::GenStatusFrom(400));
}

void Servlet::doGet(std::shared_ptr<HttpRequest> request,
                    std::shared_ptr<HttpResponse> response)
{
    std::string resource;
    try
    {
        std::ifstream stream(m_root + request->path(), std::ifstream::in);
        // 资源不存在，无法打开文件
        if (!stream.is_open())
        {
            response->status(o7si::net::GenStatusFrom(404)); 
            return;
        }

        // 读取资源
        resource = {
            std::istreambuf_iterator<char>(stream),
            std::istreambuf_iterator<char>()
        };

        stream.close();
    }
    catch (...)
    {
        // 读取资源时出现异常    
        response->status(o7si::net::GenStatusFrom(404)); 
        return;
    }

    // 成功读取资源
    response->status(o7si::net::GenStatusFrom(200));
    response->body(resource); 
}

void Servlet::doPost(std::shared_ptr<HttpRequest> request,
                     std::shared_ptr<HttpResponse> response)
{
    response->status(o7si::net::GenStatusFrom(404));   
}

// ----------------------------------------------------------------------------


}   // namespace net    
}   // namespace o7si
