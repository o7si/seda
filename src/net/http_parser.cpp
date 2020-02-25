#include "http_parser.h"

namespace o7si
{
namespace net
{

void request_field_cb(void *data, const char* field, size_t flen, const char* value, size_t vlen)
{
    LOG_DEBUG << std::string(field, flen) << " " << std::string(value, vlen);

    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    parser->data()->set_field(std::string(field, flen), std::string(value, vlen));
}
   
void request_method_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);

    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    parser->data()->method(GenMethodFrom(std::string(at, length)));
}

void request_uri_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);

    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    parser->data()->uri(std::string(at, length));
}


void request_fragment_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);

    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    parser->data()->fragment(std::string(at, length));
}

void request_path_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);

    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    parser->data()->path(std::string(at, length));
}

void request_query_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);

    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    parser->data()->query(std::string(at, length));
}

void request_version_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);

    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    parser->data()->version(GenVersionFrom(std::string(at, length)));
}

void request_header_done_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
}

void response_field_cb(void* data, const char* field, size_t flen,
                                   const char* value, size_t vlen)
{
    LOG_DEBUG << std::string(field, flen) << " " << std::string(value, vlen);
}

void response_reason_phrase_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}

void response_status_code_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}

void response_chunk_size_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}


void response_version_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}

void response_header_done_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}

void response_last_chunk_cb(void* data, const char* at, size_t length)
{
    LOG_DEBUG << std::string(at, length);
}

HttpRequestParser::HttpRequestParser()
{
    m_request = std::make_shared<HttpRequest>();
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

    m_parser.data = this;
}

size_t HttpRequestParser::execute(const char* data, size_t len, size_t off)
{
    LOG_DEBUG << "execute";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return http_parser_execute(&m_parser, data, len, off);
}

HttpResponseParser::HttpResponseParser()
{
    //m_response = std::make_shared<HttpResponse>();
    // 初始化内部解析器
    httpclient_parser_init(&m_parser);
    // 绑定回调函数
    m_parser.http_field = response_field_cb;
    m_parser.reason_phrase = response_reason_phrase_cb;
    m_parser.status_code = response_status_code_cb;
    m_parser.chunk_size = response_chunk_size_cb;
    m_parser.http_version = response_version_cb;
    m_parser.header_done = response_header_done_cb;
    m_parser.last_chunk = response_last_chunk_cb; 
}

size_t HttpResponseParser::execute(const char* data, size_t len, size_t off)
{
    return httpclient_parser_execute(&m_parser, data, len, off);
}

}   // namespace net    
}   // namespace o7si
