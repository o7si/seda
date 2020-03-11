#include <net/sockaddr.h>

int main(int argc, char* argv[])
{
    using namespace o7si::net;
    std::shared_ptr<SockAddrIn> in = SockAddrIn::LocalHost(1000);
    std::cout << *in << std::endl;

    std::shared_ptr<SockAddrIn6> in6 = SockAddrIn6::LocalHost(1000);
    std::cout << *in6 << std::endl;

    std::shared_ptr<SockAddrUn> un = SockAddrUn::LocalHost("test.socket");
    std::cout << *un << std::endl;
    return 0;   
}
