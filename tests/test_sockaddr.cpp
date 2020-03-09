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

void test_set_c_data()
{
    {
        sockaddr_storage storage;    
        memset(&storage, 0, sizeof(storage));

        ((sockaddr_in*)&storage)->sin_family = AF_INET;
        ((sockaddr_in*)&storage)->sin_port = htons(1234);
        inet_pton(AF_INET, "1.2.3.4", &((sockaddr_in*)&storage)->sin_addr.s_addr);

        o7si::net::SockAddrIn in;
        in.set_c_data((sockaddr*)&storage);
    
        LOG_DEBUG_SYS << "addr = " << in.getAddr();
        LOG_DEBUG_SYS << "port = " << in.getPort();
    }
    {
        sockaddr_storage storage;    
        memset(&storage, 0, sizeof(storage));

        ((sockaddr_in6*)&storage)->sin6_family = AF_INET6;
        ((sockaddr_in6*)&storage)->sin6_port = htons(1234);
        inet_pton(AF_INET6, "::", &((sockaddr_in6*)&storage)->sin6_addr.s6_addr);

        o7si::net::SockAddrIn6 in6;
        in6.set_c_data((sockaddr*)&storage);
    
        LOG_DEBUG_SYS << "addr = " << in6.getAddr();
        LOG_DEBUG_SYS << "port = " << in6.getPort();
    }
    {
        sockaddr_storage storage;    
        memset(&storage, 0, sizeof(storage));

        ((sockaddr_un*)&storage)->sun_family = AF_UNIX;
        strcpy(((sockaddr_un*)&storage)->sun_path, "test.socket");

        o7si::net::SockAddrUn un;
        un.set_c_data((sockaddr*)&storage);
    
        LOG_DEBUG_SYS << "path = " << un.getPath();
    }
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
    
    // set_c_data
    test_set_c_data();
    return 0;
}
