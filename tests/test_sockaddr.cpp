#include <net/sockaddr.h>
#include <log.h>

void test_sockaddr_in(const char* addr, const char* port)
{
    o7si::net::Lookup lookup;
    lookup.lookup(addr);
    if (!lookup.has4())
    {
        LOG_DEBUG_SYS << "No IPv4.";
        return;    
    }

    o7si::net::SockAddrIn sockaddr(lookup.get4()[0].getAddr(), port);
    LOG_DEBUG_CHECK_FALSE_SYS(sockaddr.is_work()) << "may not work.";
    LOG_DEBUG_SYS << "addr = " << sockaddr.getAddr();
    LOG_DEBUG_SYS << "port = " << sockaddr.getPort();
}

void test_sockaddr_in6(const char* addr, const char* port)
{
    o7si::net::Lookup lookup;
    lookup.lookup(addr);
    if (!lookup.has6())
    {
        LOG_DEBUG_SYS << "No IPv6.";
        return;    
    }

    o7si::net::SockAddrIn6 sockaddr(lookup.get6()[0].getAddr(), port);
    LOG_DEBUG_CHECK_FALSE_SYS(sockaddr.is_work()) << "may not work.";
    LOG_DEBUG_SYS << "addr = " << sockaddr.getAddr();
    LOG_DEBUG_SYS << "port = " << sockaddr.getPort();
}

void test_sockaddr_un(const char* path)
{
    o7si::net::SockAddrUn sockaddr(path);
    LOG_DEBUG_CHECK_FALSE_SYS(sockaddr.is_work()) << "may not work.";
    LOG_DEBUG_SYS << "path = " << sockaddr.getPath();
}

int main(int argc, char* argv[])
{
    // work
    test_sockaddr_in("www.google.com", "80");
    test_sockaddr_in6("www.google.com", "80");
    test_sockaddr_un("/tmp/test.socket");
    
    // not work
    test_sockaddr_in("www.google.com", "80000");
    test_sockaddr_in6("www.google.com", "80000");
    test_sockaddr_un(std::string(200, 'a').data());
    return 0;
}
