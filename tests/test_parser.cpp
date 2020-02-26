#include <iostream>
#include "net/http_parser.h"

const char* test_request = 
"POST / HTTP/1.1\r\n"
"Host: hreport.meituan.com\r\n"
"Content-Type: application/json; charset=utf-8\r\n"
"Cookie: _lx_utm=utm_term%3D10.6.401%26utm_source%3DAppStore%26utm_content%3D000000000000001F5D2E5AD75464FBCB9E7E9760C6E9DA154361341062100633%26utm_medium%3Diphone%26utm_campaign%3DAgroupBgroupD100H0; _lxsdk_s=1707bbb2cac-911-c89-b71%7C1082106281%7C1; _utm_campaign=AgroupBgroupD100H0; _utm_content=000000000000001F5D2E5AD75464FBCB9E7E9760C6E9DA154361341062100633; _utm_medium=iphone; _utm_source=AppStore; _utm_term=10.6.401; cityid=1; dpid=; latlng=40.07929%2C116.36320%2C1582623844993; network=4g; token=ZntYZXDN_CvTrBOHjOslM090H7UGAAAAEAoAAPHvmj4bpHj31BivDa3MrzqNSGvwmWX1O02undGzwuT3X9ScihHD6KPDLuvqGp1boQ; uuid=000000000000001F5D2E5AD75464FBCB9E7E9760C6E9DA154361341062100633; latlon=40.07929%2C116.36320%2C1582623842523; _hc.v=4d28aa62-46c4-154a-d485-4ea12d6b7d74.1576648287; ci=1; cityname=%E5%8C%97%E4%BA%AC; iuuid=000000000000001F5D2E5AD75464FBCB9E7E9760C6E9DA154361341062100633; _lxsdk=000000000000001F5D2E5AD75464FBCB9E7E9760C6E9DA154361341062100633; _lxsdk_cuid=16ea5209cbec8-00c0fc790c5fdb-59402607-4a574-16ea5209cbec8; _lxsdk_unoinid=01f5d2e5ad75464fbcb9e7e9760c6e9da154361341062100633\r\n"
"Connection: keep-alive\r\n"
"Accept: */*\r\n"
"Content-Encoding: gzip\r\n"
"Accept-Language: zh-cn\r\n"
"Content-Length: 410\r\n"
"Accept-Encoding: gzip, deflate\r\n"
"User-Agent: %E7%BE%8E%E5%9B%A2/48510 CFNetwork/1121.2.2 Darwin/19.3.0\r\n";

const char* test_response = 
"HTTP/1.1 301 Moved Permanently\r\n"
"Server: nginx/1.4.6 (Ubuntu)\r\n"
"Date: Wed, 26 Feb 2020 14:13:15 GMT\r\n"
"Content-Type: text/html\r\n"
"Content-Length: 193\r\n"
"Connection: keep-alive\r\n"
"Location: https://xxx.cn/\r\n"
"\r\n"
"<html>\r\n"
"<head><title>301 Moved Permanently</title></head>\r\n"
"<body bgcolor=\"white\">\r\n"
"<center><h1>301 Moved Permanently</h1></center>\r\n"
"<hr><center>nginx/1.4.6 (Ubuntu)</center>\r\n"
"</body>\r\n"
"</html>\r\n";

int main(int argc, char* argv[])
{
    using namespace o7si::net;

    HttpRequestParser parser1;
    std::string req_str = test_request;
    parser1.parser_execute(req_str.data(), req_str.size());
    std::cout << parser1.inner_request()->format() << std::endl;

    std::cout << "---";  

    HttpResponseParser parser2;
    std::string res_str = test_response;
    parser2.parser_execute(res_str.data(), res_str.size());
    std::cout << parser2.inner_response()->format() << std::endl;
    return 0;    
}
