#include "http_parser.h"

namespace o7si
{
namespace net
{

void request_field_cb(void *data, const char* field, size_t flen, const char* value, size_t vlen)
{
    LOG_DEBUG << std::string(field, flen) << " " << std::string(value, vlen);
}
   
void request_method_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}

void request_uri_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}


void request_fragment_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}

void request_path_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}

void request_query_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}

void request_version_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}

void request_header_done_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}

HttpRequestParser::HttpRequestParser()
{
    // 初始化解析器
    http_parser_init(&m_parser);
    // 绑定回调函数
    m_parser.http_field = request_field_cb;
    m_parser.request_method = request_method_cb;
    m_parser.request_uri = request_uri_cb;     
    m_parser.fragment = request_fragment_cb;
    m_parser.request_path = request_path_cb;
    m_parser.query_string = request_query_cb;
    m_parser.http_version = request_version_cb;
    m_parser.header_done = request_header_done_cb;
}

size_t HttpRequestParser::execute(const char* data, size_t len, size_t off)
{
    return http_parser_execute(&m_parser, data, len, off);
}

}   // namespace net    
}   // namespace o7si
