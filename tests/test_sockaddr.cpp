#include <net/sockaddr.h>
#include <log.h>

#include <climits>

void test01()
{
    using namespace o7si::net;
    SockAddrIn sockaddr("", 0);

    // 正常情况
    sockaddr.setPort(12345);
    LOG_DEBUG_SYS << sockaddr.getPort();
    sockaddr.setPort("54321");
    LOG_DEBUG_SYS << sockaddr.getPort();

    sockaddr.setAddr(INADDR_ANY);
    LOG_DEBUG_SYS << sockaddr.getAddr();
    sockaddr.setAddrDot("116.62.15.145");
    LOG_DEBUG_SYS << sockaddr.getAddr();
    sockaddr.setAddrDomain("www.baidu.com");
    LOG_DEBUG_SYS << sockaddr.getAddr();

    // 异常情况
    sockaddr.setPort(123456);
    LOG_WARN_SYS << sockaddr.getPort();

    sockaddr.setAddr(INT_MAX);
    LOG_WARN_SYS << sockaddr.getAddr();
    sockaddr.setAddrDot("256.62.15.145");
    LOG_WARN_SYS << sockaddr.getAddr();
    sockaddr.setAddrDomain("www.o7si.com");
    LOG_WARN_SYS << sockaddr.getAddr();
}

void test02()
{
    using namespace o7si::net;
    SockAddrIn sockaddr("www.baidu.com", 80, 1);
    
    LOG_DEBUG_SYS << sockaddr.getAddr();
    LOG_DEBUG_SYS << sockaddr.getPort();    
}

int main(int argc, char* argv[])
{
    test02();
    return 0;    
}
