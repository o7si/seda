#include <oseda/seda.h>


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        LOG_ERROR_SYS << "e.g.";    
        LOG_ERROR_SYS << argv[0] << " 10000";    
        return EXIT_FAILURE;
    }

    using namespace o7si::net;

    int port = atoi(argv[1]);
    auto cli_socket = UDPClientSocket::GenSocket(SockAddrIn::LocalHost(port));

    while (true)
    {
        std::string send;
        std::cin >> send;  
        int send_size = cli_socket->sendto(send);

        if (send == "quit")
            break;

        std::shared_ptr<SockAddr> from;
        std::string recv;
        int recv_size = cli_socket->recvfrom(from, recv); 
        LOG_DEBUG_SYS << "from: " << from->format();
        LOG_DEBUG_SYS << "<server-say> " << recv; 
    }
    cli_socket->close();
    return EXIT_SUCCESS;
}
