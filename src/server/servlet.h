/******************************************************************************
 * File: servlet.h
 * Description: 负责 Web 请求和响应的处理。
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <memory>
#include <string>

#include "../log.h"
#include "../net/http.h"

namespace o7si
{
namespace net
{


// ----------------------------------------------------------------------------

// 约定（仅支持 GET 和 POST 请求）
//
// HTTP Method 为 GET 时，表示请求网页、图片等资源
// HTTP Method 为 POST 时，表示请求服务器接口
//
//
//                                         |--[处理成功：200]
//                          |--[资源存在]--|
//                          |              |--[处理失败：500]
//                |--[GET]--|
//                |         |
//                |         |--[资源不存在：404]
//                |
// [HTTPRequest]--|
//                |                        |--[处理成功：200]
//                |         |--[接口存在]--|
//                |         |              |--[处理失败：500]
//                |--[POST]-|
//                |         |
//                |         |--[接口不存在：404]
//                |
//                |--[其他：400]

class Servlet
{
public:
    explicit Servlet(std::string root);

    // 处理 HTTP 请求
    void todo(std::shared_ptr<HttpRequest> request, 
              std::shared_ptr<HttpResponse> response);

    void doGet(std::shared_ptr<HttpRequest> request,
               std::shared_ptr<HttpResponse> response);

    void doPost(std::shared_ptr<HttpRequest> request,
                std::shared_ptr<HttpResponse> response);

private:
    // 页面的根路径
    std::string m_root;
};

// ----------------------------------------------------------------------------
    

}   // namespace net    
}   // nemespace o7si
