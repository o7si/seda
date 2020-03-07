#include <net/sockaddr.h>
#include <log.h>

void test01(const char* addr, const char* port)
{
    o7si::net::Lookup lookup;
    lookup.lookup(addr);

    if (lookup.has4())
    {
        LOG_DEBUG_SYS << "IPv4 Test";
        o7si::net::SockAddrIn in4(lookup.get4()[0].getAddr(), port);    
        LOG_DEBUG_SYS << "addr = " << in4.getAddr();
        LOG_DEBUG_SYS << "port = " << in4.getPort();
    }

    if (lookup.has6())
    {
        LOG_DEBUG_SYS << "IPv6 Test";
        o7si::net::SockAddrIn6 in6(lookup.get6()[0].getAddr(), port);    
        LOG_DEBUG_SYS << "addr = " << in6.getAddr();
        LOG_DEBUG_SYS << "port = " << in6.getPort();
    }
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        LOG_ERROR_SYS << "please input address and port.";
        LOG_ERROR_SYS << "e.g.";
        LOG_ERROR_SYS << argv[0] << " " << "www.google.com 80";
        return EXIT_FAILURE;
    }
       
    test01(argv[1], argv[2]);
    return EXIT_SUCCESS;    
}
