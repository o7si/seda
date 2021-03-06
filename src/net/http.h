/******************************************************************************
 * File: http.h
 * Description: Http 请求以及响应的定义。
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <strings.h>

#include <iostream>
#include <map>
#include <sstream>
#include <string>


namespace o7si
{
namespace net
{

// ----------------------------------------------------------------------------    

// HTTP 状态
#define HTTP_STATUS_MAP(XX)                                                     \
    XX(100, CONTINUE,                        Continue)                          \
    XX(101, SWITCHING_PROTOCOLS,             Switching Protocols)               \
    XX(102, PROCESSING,                      Processing)                        \
    XX(200, OK,                              OK)                                \
    XX(201, CREATED,                         Created)                           \
    XX(202, ACCEPTED,                        Accepted)                          \
    XX(203, NON_AUTHORITATIVE_INFORMATION,   Non-Authoritative Information)     \
    XX(204, NO_CONTENT,                      No Content)                        \
    XX(205, RESET_CONTENT,                   Reset Content)                     \
    XX(206, PARTIAL_CONTENT,                 Partial Content)                   \
    XX(207, MULTI_STATUS,                    Multi-Status)                      \
    XX(208, ALREADY_REPORTED,                Already Reported)                  \
    XX(226, IM_USED,                         IM Used)                           \
    XX(300, MULTIPLE_CHOICES,                Multiple Choices)                  \
    XX(301, MOVED_PERMANENTLY,               Moved Permanently)                 \
    XX(302, FOUND,                           Found)                             \
    XX(303, SEE_OTHER,                       See Other)                         \
    XX(304, NOT_MODIFIED,                    Not Modified)                      \
    XX(305, USE_PROXY,                       Use Proxy)                         \
    XX(307, TEMPORARY_REDIRECT,              Temporary Redirect)                \
    XX(308, PERMANENT_REDIRECT,              Permanent Redirect)                \
    XX(400, BAD_REQUEST,                     Bad Request)                       \
    XX(401, UNAUTHORIZED,                    Unauthorized)                      \
    XX(402, PAYMENT_REQUIRED,                Payment Required)                  \
    XX(403, FORBIDDEN,                       Forbidden)                         \
    XX(404, NOT_FOUND,                       Not Found)                         \
    XX(405, METHOD_NOT_ALLOWED,              Method Not Allowed)                \
    XX(406, NOT_ACCEPTABLE,                  Not Acceptable)                    \
    XX(407, PROXY_AUTHENTICATION_REQUIRED,   Proxy Authentication Required)     \
    XX(408, REQUEST_TIMEOUT,                 Request Timeout)                   \
    XX(409, CONFLICT,                        Conflict)                          \
    XX(410, GONE,                            Gone)                              \
    XX(411, LENGTH_REQUIRED,                 Length Required)                   \
    XX(412, PRECONDITION_FAILED,             Precondition Failed)               \
    XX(413, PAYLOAD_TOO_LARGE,               Payload Too Large)                 \
    XX(414, URI_TOO_LONG,                    URI Too Long)                      \
    XX(415, UNSUPPORTED_MEDIA_TYPE,          Unsupported Media Type)            \
    XX(416, RANGE_NOT_SATISFIABLE,           Range Not Satisfiable)             \
    XX(417, EXPECTATION_FAILED,              Expectation Failed)                \
    XX(421, MISDIRECTED_REQUEST,             Misdirected Request)               \
    XX(422, UNPROCESSABLE_ENTITY,            Unprocessable Entity)              \
    XX(423, LOCKED,                          Locked)                            \
    XX(424, FAILED_DEPENDENCY,               Failed Dependency)                 \
    XX(426, UPGRADE_REQUIRED,                Upgrade Required)                  \
    XX(428, PRECONDITION_REQUIRED,           Precondition Required)             \
    XX(429, TOO_MANY_REQUESTS,               Too Many Requests)                 \
    XX(431, REQUEST_HEADER_FIELDS_TOO_LARGE, Request Header Fields Too Large)   \
    XX(451, UNAVAILABLE_FOR_LEGAL_REASONS,   Unavailable For Legal Reasons)     \
    XX(500, INTERNAL_SERVER_ERROR,           Internal Server Error)             \
    XX(501, NOT_IMPLEMENTED,                 Not Implemented)                   \
    XX(502, BAD_GATEWAY,                     Bad Gateway)                       \
    XX(503, SERVICE_UNAVAILABLE,             Service Unavailable)               \
    XX(504, GATEWAY_TIMEOUT,                 Gateway Timeout)                   \
    XX(505, HTTP_VERSION_NOT_SUPPORTED,      HTTP Version Not Supported)        \
    XX(506, VARIANT_ALSO_NEGOTIATES,         Variant Also Negotiates)           \
    XX(507, INSUFFICIENT_STORAGE,            Insufficient Storage)              \
    XX(508, LOOP_DETECTED,                   Loop Detected)                     \
    XX(510, NOT_EXTENDED,                    Not Extended)                      \
    XX(511, NETWORK_AUTHENTICATION_REQUIRED, Network Authentication Required)   \


enum class HttpStatus
{
// e.g. HTTP_STATUS_OK = 200
#define XX(num, name, string) HTTP_STATUS_##name = num,
    HTTP_STATUS_MAP(XX)
#undef XX  
    /* 未定义 */
    HTTP_STATUS_UNDEFINE
};

// ----------------------------------------------------------------------------    

// HTTP 方法 
#define HTTP_METHOD_MAP(XX)             \
    XX(0,  DELETE,      DELETE)         \
    XX(1,  GET,         GET)            \
    XX(2,  HEAD,        HEAD)           \
    XX(3,  POST,        POST)           \
    XX(4,  PUT,         PUT)            \
    /* pathological */                  \
    XX(5,  CONNECT,     CONNECT)        \
    XX(6,  OPTIONS,     OPTIONS)        \
    XX(7,  TRACE,       TRACE)          \
    /* WebDAV */                        \
    XX(8,  COPY,        COPY)           \
    XX(9,  LOCK,        LOCK)           \
    XX(10, MKCOL,       MKCOL)          \
    XX(11, MOVE,        MOVE)           \
    XX(12, PROPFIND,    PROPFIND)       \
    XX(13, PROPPATCH,   PROPPATCH)      \
    XX(14, SEARCH,      SEARCH)         \
    XX(15, UNLOCK,      UNLOCK)         \
    XX(16, BIND,        BIND)           \
    XX(17, REBIND,      REBIND)         \
    XX(18, UNBIND,      UNBIND)         \
    XX(19, ACL,         ACL)            \
    /* subversion */                    \
    XX(20, REPORT,      REPORT)         \
    XX(21, MKACTIVITY,  MKACTIVITY)     \
    XX(22, CHECKOUT,    CHECKOUT)       \
    XX(23, MERGE,       MERGE)          \
    /* upnp */                          \
    XX(24, MSEARCH,     M-SEARCH)       \
    XX(25, NOTIFY,      NOTIFY)         \
    XX(26, SUBSCRIBE,   SUBSCRIBE)      \
    XX(27, UNSUBSCRIBE, UNSUBSCRIBE)    \
    /* RFC-5789 */                      \
    XX(28, PATCH,       PATCH)          \
    XX(29, PURGE,       PURGE)          \
    /* CalDAV */                        \
    XX(30, MKCALENDAR,  MKCALENDAR)     \
    /* RFC-2068, section 19.6.1.2 */    \
    XX(31, LINK,        LINK)           \
    XX(32, UNLINK,      UNLINK)         \
    /* icecast */                       \
    XX(33, SOURCE,      SOURCE)         \


enum class HttpMethod 
{
// e.g. HTTP_METHOD_DELETE = 0
#define XX(num, name, string) HTTP_METHOD_##name = num,
    HTTP_METHOD_MAP(XX)
#undef XX
    /* 未定义 */
    HTTP_METHOD_UNDEFINE
};

// ----------------------------------------------------------------------------    

// HTTP 版本 
#define HTTP_VERSION_MAP(XX)    \
    XX(10, 1_0, HTTP/1.0)       \
    XX(11, 1_1, HTTP/1.1)       \
    XX(20, 2_0, HTTP/2.0)       \


enum class HttpVersion
{
// e.g. HTTP_VERSION_1_0 = 10
#define XX(num, name, string) HTTP_VERSION_##name = num,
    HTTP_VERSION_MAP(XX)
#undef XX    
    /* 未定义 */
    HTTP_VERSION_UNDEFINE
};

// ----------------------------------------------------------------------------    

// 根据 XX 生成对应的 HTTP Status
HttpStatus GenStatusFrom(const std::string& str);
HttpStatus GenStatusFrom(const char* str);
HttpStatus GenStatusFrom(int number);

// 根据 XX 生成对应的 HTTP Method
HttpMethod GenMethodFrom(const std::string& str);
HttpMethod GenMethodFrom(const char* str);
HttpMethod GenMethodFrom(int number);

// 根据 XX 生成对应的 HTTP Version
HttpVersion GenVersionFrom(const std::string& str);
HttpVersion GenVersionFrom(const char* str);
HttpVersion GenVersionFrom(int number);

// 将数据转换为字符串
std::string HttpStatusToString(const HttpStatus& status);
std::string HttpMethodToString(const HttpMethod& method);
std::string HttpVersionToString(const HttpVersion& version);

// 输出相关的描述(用作调试)
std::ostream& operator<<(std::ostream& stream, const HttpStatus& status);
std::ostream& operator<<(std::ostream& stream, const HttpMethod& method);
std::ostream& operator<<(std::ostream& string, const HttpVersion& version);

//-------------------------------------------------------------------------------    

// HTTP 请求
class HttpRequest
{
public:
    using MapType = std::map<std::string, std::string>;
public:
    HttpMethod method() const
    {
        return m_method;    
    }

    void method(const HttpMethod& value)
    {
        m_method = value;    
    }

    HttpVersion version() const
    {
        return m_version;    
    }

    void version(const HttpVersion& value)
    {
        m_version = value;
    }
    
    std::string uri() const
    {
        return m_uri; 
    }

    void uri(const std::string& value)
    {
        m_uri = value;
    }

    std::string path() const
    {
        return m_path;    
    }

    void path(const std::string& value)
    {
        m_path = value;
    }

    std::string query() const
    {
        return m_query;    
    }

    void query(const std::string& value)
    {
        m_query = value;
    } 

    std::string fragment() const
    {
        return m_fragment;    
    }

    void fragment(const std::string& value)
    {
        m_fragment = value;
    }

    std::string body() const
    {
        return m_body;    
    }

    void body(const std::string& value)
    {
        m_body = value;
    }
    
    // 获取字段
    MapType fields() const
    {
        return m_fields;    
    }

    // 设置字段
    void fields(const MapType& value)
    {
        m_fields = value;    
    }

    // 判断字段是否存在
    bool has_field(const std::string& key)
    {
        return m_fields.find(key) != m_fields.end();    
    }

    // 获取字段，若字段不存在则返回默认值
    std::string get_field(const std::string& key, const std::string& def = "")
    {
        // 字段不存在
        if (!has_field(key))
            return def;
        // 字段存在
        return m_fields[key]; 
    }

    // 设置一个字段
    void set_field(const std::string& key, const std::string& value)
    {
        m_fields[key] = value;
    }

    // 移除一个字段
    void remove_field(const std::string& key)
    {
        // 字段不存在
        if (!has_field(key))
            return;
        m_fields.erase(m_fields.find(key));
    }
    
    // 格式化数据
    std::string format();

    // 将数据格式化并输出至流
    std::ostream& dump(std::ostream& stream);

private:
    HttpMethod m_method;
    HttpVersion m_version;
    
    std::string m_uri;
    std::string m_path;
    std::string m_query;
    std::string m_fragment;
    std::string m_body;
    
    MapType m_fields;
};

// HTTP 响应报文
class HttpResponse
{
public:
    using MapType = std::map<std::string, std::string>;

public:

    HttpStatus status() const
    {
        return m_status;    
    }

    void status(const HttpStatus& status)
    {
        m_status = status;    
    }

    HttpVersion version() const
    {
        return m_version;    
    } 

    void version(const HttpVersion& version)
    {
        m_version = version;    
    }

    std::string reason_phrase() const
    {
        return m_reason_phrase;
    }

    void reason_phrase(const std::string& value)
    {
        m_reason_phrase = value;
    }

    std::string body() const
    {
        return m_body;    
    }

    void body(const std::string& value)
    {
        m_body = value;
    }
    
    // 获取字段
    MapType fields() const
    {
        return m_fields;    
    }

    // 设置字段
    void fields(const MapType& value)
    {
        m_fields = value;    
    }

    // 判断字段是否存在
    bool has_field(const std::string& key)
    {
        return m_fields.find(key) != m_fields.end();    
    }

    // 获取字段，若字段不存在则返回默认值
    std::string get_field(const std::string& key, const std::string& def = "")
    {
        // 字段不存在
        if (!has_field(key))
            return def;
        // 字段存在
        return m_fields[key]; 
    }

    // 设置一个字段
    void set_field(const std::string& key, const std::string& value)
    {
        m_fields[key] = value;
    }

    // 移除一个字段
    void remove_field(const std::string& key)
    {
        // 字段不存在
        if (!has_field(key))
            return;
        m_fields.erase(m_fields.find(key));
    }
    
    // 格式化数据
    std::string format();

    // 将数据格式化并输出至流
    std::ostream& dump(std::ostream& stream);

private:    
    HttpStatus m_status;
    HttpVersion m_version;

    std::string m_reason_phrase;
    std::string m_body;

    MapType m_fields;
};

// ----------------------------------------------------------------------------

}   // namespace net   
}   // namespace o7si
