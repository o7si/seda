#include "http.h"

namespace o7si
{
namespace net
{

//-------------------------------------------------------------------------------    

HttpStatus GenStatusFrom(const std::string& str)
{
    return GenStatusFrom(str.data()); 
}

HttpStatus GenStatusFrom(const char* str)
{
#define XX(num, name, string) \
    if (strcasecmp(str, #name) == 0) \
        return HttpStatus::HTTP_STATUS_##name;   
    HTTP_STATUS_MAP(XX)
#undef XX
    return HttpStatus::HTTP_STATUS_UNDEFINE;
}

HttpStatus GenStatusFrom(int number)
{
#define XX(num, name, string) \
    if (number == num) \
        return HttpStatus::HTTP_STATUS_##name;
    HTTP_STATUS_MAP(XX)
#undef XX    
    return HttpStatus::HTTP_STATUS_UNDEFINE;
}

HttpMethod GenMethodFrom(const std::string& str)
{
    return GenMethodFrom(str.data());    
}

HttpMethod GenMethodFrom(const char* str)
{
#define XX(num, name, string) \
    if (strcasecmp(str, #name) == 0) \
        return HttpMethod::HTTP_METHOD_##name;
    HTTP_METHOD_MAP(XX)
#undef XX   
    return HttpMethod::HTTP_METHOD_UNDEFINE;
}

HttpMethod GenMethodFrom(int number)
{
#define XX(num, name, string) \
    if (number == num) \
        return HttpMethod::HTTP_METHOD_##name;
    HTTP_METHOD_MAP(XX)
#undef XX    
    return HttpMethod::HTTP_METHOD_UNDEFINE;
}

HttpVersion GenVersionFrom(const std::string& str)
{
    return GenVersionFrom(str.data());    
}

HttpVersion GenVersionFrom(const char* str)
{
#define XX(num, name, string) \
    if (strcasecmp(str, #name) == 0) \
        return HttpVersion::HTTP_VERSION_##name;    
    HTTP_VERSION_MAP(XX)
#undef XX
    return HttpVersion::HTTP_VERSION_UNDEFINE;
}

HttpVersion GenVersionFrom(int number)
{
#define XX(num, name, string) \
    if (number == num) \
        return HttpVersion::HTTP_VERSION_##name;    
    HTTP_VERSION_MAP(XX)
#undef XX
    return HttpVersion::HTTP_VERSION_UNDEFINE;
}

std::ostream& operator<<(std::ostream& stream, const HttpStatus& status)
{
#define XX(num, name, string) \
    if (status == HttpStatus::HTTP_STATUS_##name) \
        return stream << num << " " << #name << " " << #string;
    HTTP_STATUS_MAP(XX);
#undef XX
    return stream << "HTTP_STATUS_UNDEFINE";
}

std::ostream& operator<<(std::ostream& stream, const HttpMethod& method)
{
#define XX(num, name, string) \
    if (method == HttpMethod::HTTP_METHOD_##name) \
        return stream << num << " " << #name << " " << #string;    
    HTTP_METHOD_MAP(XX)
#undef XX
    return stream << "HTTP_METHOD_UNDEFINE";
}

std::ostream& operator<<(std::ostream& stream, const HttpVersion& version)
{
#define XX(num, name, string) \
    if (version == HttpVersion::HTTP_VERSION_##name) \
        return stream << num << " " << #name << " " << #string;    
    HTTP_VERSION_MAP(XX)
#undef XX
    return stream << "HTTP_VERSION_UNDEFINE";
}

//-------------------------------------------------------------------------------    

}   // namespace net
}   // namespace o7si
