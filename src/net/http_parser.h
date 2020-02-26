#pragma once

#include <net/analysis/http11_parser.h>
#include <net/analysis/httpclient_parser.h>

#include "http.h"
#include "../log.h"

namespace o7si
{
namespace net
{

//--------------------------------------------------------------------------    

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

//--------------------------------------------------------------------------    

class HttpParser
{
public:
    virtual int parser_init() = 0;
    virtual int parser_finish() = 0;
    virtual int parser_has_error() = 0;
    virtual int parser_is_finished() = 0;
    virtual size_t parser_execute(const char* data, size_t len, size_t off) = 0;

    virtual ~HttpParser() = default;    
};

/// HTTP 请求解析器
class HttpRequestParser : public HttpParser
{
public:
    HttpRequestParser();

    int parser_init() override;
    int parser_finish() override;
    int parser_has_error() override;
    int parser_is_finished() override;
    size_t parser_execute(const char* data, size_t len, size_t off = 0) override;

    std::shared_ptr<HttpRequest> inner_request()
    {
        return m_request;    
    }

private:    
    http_parser m_parser;
    std::shared_ptr<HttpRequest> m_request;
};

/// HTTP 响应解析器
class HttpResponseParser : public HttpParser
{
public:
    HttpResponseParser();

    int parser_init() override;
    int parser_finish() override;
    int parser_has_error() override;
    int parser_is_finished() override;
    size_t parser_execute(const char* data, size_t len, size_t off = 0) override;

    std::shared_ptr<HttpResponse> inner_response()
    {
        return m_response;
    }

private:
    httpclient_parser m_parser;
    std::shared_ptr<HttpResponse> m_response;
};

}    
}
