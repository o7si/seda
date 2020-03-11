#include <net/sockaddr.h>
#include <net/socket.h>
#include <log.h>

int main(int argc, char* argv[])
{
    using namespace o7si::net;
    std::shared_ptr<SockAddr> sockaddr = SockAddrIn6::LocalHost(10000);
    std::shared_ptr<TCPServerSocket> server_socket = TCPServerSocket::GenSocket(sockaddr);
    int cli_fd = server_socket->accept();
    while (true)
    {
        std::string recv;
        int recv_size = server_socket->read(cli_fd, recv);
        std::cout << "server-recv-data-len = " << recv_size << std::endl;
        std::cout << "<client-say> " << recv << std::endl;

        time_t t = time(nullptr);
        std::string send = asctime(localtime(&t));
        int send_size = server_socket->write(cli_fd, send);
        std::cout << "send-msg-size = " << send_size << std::endl;
    }
    return 0;    
}
