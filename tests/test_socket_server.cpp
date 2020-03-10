#include <net/sockaddr.h>
#include <net/socket.h>
#include <log.h>

int main(int argc, char* argv[])
{
    using namespace o7si::net;
    //std::shared_ptr<SockAddr> sockaddr = std::make_shared<SockAddrIn>
    //    ("127.0.0.1", 10000);
    std::shared_ptr<SockAddr> sockaddr = SockAddrIn::LocalHost(10000);
    std::shared_ptr<TCPServerSocket> server_socket = TCPServerSocket::GenSocket(sockaddr);

    LOG_DEBUG_SYS << "wait...";
    int cli_fd = server_socket->accept();
    LOG_DEBUG_SYS << "conn_fd = " << cli_fd;
    while (true)
    {
        char buf[BUFSIZ];
        int size = server_socket->recv(cli_fd, buf, sizeof(buf));    

        if (size == -1)
            break;

        std::string recv(buf, size);
        if (recv == "quit")
            break;

        std::cout << "<client-say> " << recv << std::endl;

        std::string send = std::to_string(rand());
        server_socket->send(cli_fd, send.data(), send.size());
    }
    return 0;    
}
