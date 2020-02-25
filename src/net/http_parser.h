#pragma once

#include <net/analysis/http11_parser.h>
#include <net/analysis/httpclient_parser.h>

#include "http.h"
#include "../log.h"

namespace o7si
{
namespace net
{
    
void request_field_cb(void* data, const char* field, size_t flen, 
                                  const char* value, size_t vlen);
void request_method_cb(void* data, const char* at, size_t length);
void request_uri_cb(void* data, const char* at, size_t length);
void request_fragment_cb(void* data, const char* at, size_t length);
void request_path_cb(void* data, const char* at, size_t length); 
void request_query_cb(void* data, const char* at, size_t length); 
void request_version_cb(void* data, const char* at, size_t length); 
void request_header_done_cb(void* data, const char* at, size_t length); 

void response_field_cb(void* data, const char* field, size_t flen,
                                   const char* value, size_t vlen);
void response_reason_phrase_cb(void* data, const char* at, size_t length);
void response_status_code_cb(void* data, const char* at, size_t length);
void response_chunk_size_cb(void* data, const char* at, size_t length);
void response_version_cb(void* data, const char* at, size_t length);
void response_header_done_cb(void* data, const char* at, size_t length);
void response_last_chunk_cb(void* data, const char* at, size_t length);

/// HTTP 请求解析器
class HttpRequestParser
{
public:
    /// 构造函数
    HttpRequestParser();

    /// 执行
    size_t execute(const char* data, size_t len, size_t off = 0);

    std::shared_ptr<HttpRequest> data()
    {
        return m_request;    
    }
private:    
    http_parser m_parser;
    std::shared_ptr<HttpRequest> m_request;
};

/// HTTP 响应解析器
class HttpResponseParser
{
public:
    HttpResponseParser();
    /// 执行
    size_t execute(const char* data, size_t len, size_t off = 0);

    std::shared_ptr<HttpResponse> data()
    {
        return m_response;    
    }
private:
    httpclient_parser m_parser;
    std::shared_ptr<HttpResponse> m_response;
};

}    
}
