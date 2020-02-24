#pragma once

#include "http11_parser.h"
#include "../log.h"

namespace o7si
{
namespace net
{
    
/// HTTP 请求解析
class HttpRequestParser
{
public:
    HttpRequestParser();

    size_t execute(const char* data, size_t len, size_t off = 0);
private:    
    http_parser m_parser;
};

}    
}
