#include "servlet.h"


namespace o7si
{
namespace server
{


// ----------------------------------------------------------------------------

Servlet::Servlet(std::string root)
    : m_root(std::move(root))
{
}
    
void Servlet::todo(std::shared_ptr<o7si::net::HttpRequest> request,
                     std::shared_ptr<o7si::net::HttpResponse> response)
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

void Servlet::doGet(std::shared_ptr<o7si::net::HttpRequest> request,
                    std::shared_ptr<o7si::net::HttpResponse> response)
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

void Servlet::doPost(std::shared_ptr<o7si::net::HttpRequest> request,
                     std::shared_ptr<o7si::net::HttpResponse> response)
{
    std::string api = request->path();
    // 未知的请求接口
    if (!APIManager::Instance()->has(api))
    {
        response->status(o7si::net::GenStatusFrom(404));
        return;    
    }

    // 接口存在，调用接口
    std::string out;
    bool ret = APIManager::Instance()->call(api, request->body(), out);

    // 执行错误
    if (!ret)
    {
        response->status(o7si::net::GenStatusFrom(500));
        return;    
    }

    // 正常执行
    response->status(o7si::net::GenStatusFrom(200));
    response->body(out);
}

// ----------------------------------------------------------------------------


}   // namespace server 
}   // namespace o7si
