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
    auto ser_socket = TCPServerSocket::GenSocket(SockAddrIn::LocalHost(port));

    int cli_fd = ser_socket->accept();    
    while (true)
    {
        std::string recv;
        int recv_size = ser_socket->read(cli_fd, recv);        
        LOG_DEBUG_SYS << "<client-say> " << recv;

        if (recv == "quit")
            break;

        std::string send(recv.crbegin(), recv.crend());
        int send_size = ser_socket->write(cli_fd, send);
    }
    ser_socket->close();
    return EXIT_SUCCESS;   
}
