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
    auto ser_socket = UDPServerSocket::GenSocket(SockAddrIn::LocalHost(port));

    while (true)
    {
        std::shared_ptr<SockAddr> from;    
        std::string recv;
        size_t recv_size = ser_socket->recvfrom(from, recv);
        if (recv_size == -1)
            continue;

        LOG_DEBUG_SYS << "from: " << from->format();
        LOG_DEBUG_SYS << "<client-say> " << recv;

        if (recv == "quit")
            break;

        std::shared_ptr<SockAddr> to = from;
        std::string send(recv.crbegin(), recv.crend());
        size_t send_size = ser_socket->sendto(to, send);
    }
    ser_socket->close();
    return EXIT_SUCCESS;    
}
