#include <net/sockaddr.h>
#include <net/socket.h>
#include <log.h>

int main(int argc, char* argv[])
{
    using namespace o7si::net;
    std::shared_ptr<SockAddr> sockaddr = SockAddrIn::LocalHost(10000);
    std::shared_ptr<TCPClientSocket> client_socket = TCPClientSocket::GenSocket(sockaddr);
    client_socket->connect();
    while (true)
    {
        std::string send;
        std::cin >> send;
        int send_size = client_socket->write(send.data(), send.size());
        std::cout << "client-send-data-len = " << send_size << std::endl;

        std::string recv; 
        int recv_size = client_socket->read(recv);
        std::cout << "client-recv-data-len = " << recv_size << std::endl;
        std::cout << "<server-say> " << recv << std::endl;
    }
    return 0;    
}
