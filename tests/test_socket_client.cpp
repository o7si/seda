#include <net/sockaddr.h>
#include <net/socket.h>
#include <log.h>

int main(int argc, char* argv[])
{
    using namespace o7si::net;
    //std::shared_ptr<SockAddr> sockaddr = std::make_shared<SockAddrIn>
    //    ("127.0.0.1", 10000);
    std::shared_ptr<SockAddr> sockaddr = SockAddrIn::LocalHost(10000);
    std::shared_ptr<TCPClientSocket> client_socket = TCPClientSocket::GenSocket(sockaddr);

    bool ret = client_socket->connect();
    if (!ret)
    {
        LOG_DEBUG_SYS << "connect error";
        return EXIT_FAILURE;  
    }

    while (true)
    {
        std::string msg;
        std::cin >> msg;

        client_socket->send(msg.data(), msg.size());

        char buf[BUFSIZ];
        int size = client_socket->recv(buf, sizeof(buf));
        std::string recv(buf, size);
        std::cout << "<server-say> " << recv << std::endl;
    }
    return 0;    
}
