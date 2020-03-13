/******************************************************************************
 * File: servlet.h
 * Description: 负责 HTTP 请求和响应的处理。
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <memory>
#include <string>

#include "../log.h"
#include "../utils.h"
#include "http.h"

namespace o7si
{
namespace net
{


// ----------------------------------------------------------------------------

class Servlet
{
public:
    explicit Servlet(std::string root);

    // 处理 HTTP 请求
    void todo(std::shared_ptr<HttpRequest> request, 
              std::shared_ptr<HttpResponse> response);

private:
    // 页面的根路径
    std::string m_root;
};

// ----------------------------------------------------------------------------
    

}   // namespace net    
}   // nemespace o7si
