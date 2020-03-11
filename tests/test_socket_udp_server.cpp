#include <net/socket.h>
#include <net/sockaddr.h>

// UDP Server
// 服务端接收一个字符串，返回字符串的长度
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "please input port." << std::endl;
        std::cout << "e.g." << std::endl;
        std::cout << argv[0] << " 10000" << std::endl;    
        return EXIT_FAILURE; 
    }

    using namespace o7si::net;
    // 绑定指定的端口
    std::shared_ptr<UDPServerSocket> server_socket = 
        UDPServerSocket::GenSocket(SockAddrIn::LocalHost(atoi(argv[1])));

    server_socket->setSendTimeout(5);
    server_socket->setRecvTimeout(5);

    while (true)
    {
        std::shared_ptr<SockAddr> from;
        std::string recv_msg;
        ssize_t recv_size = server_socket->recvfrom(from, recv_msg);    

        // 如果消息接收失败，通常情况下是超时
        if (recv_size == -1)
        {
            std::cout << "recv timeout" << std::endl;
            continue;
        }

        std::cout << "recv-info" << std::endl;
        std::cout << "\tmsg  : " << recv_msg << std::endl;
        std::cout << "\tsize : " << recv_size << std::endl; 
        std::cout << "\tfrom : " << std::endl;

        if (recv_msg == "quit")
            break;

        std::shared_ptr<SockAddr> to = from;
        std::string send_msg = std::to_string(recv_msg.size()) + " bytes";
        ssize_t send_size = server_socket->sendto(to, send_msg);
        
        // 如果消息发送失败
        if (send_size == -1)
            break;

        std::cout << "send-info" << std::endl;
        std::cout << "\tmsg  : " << send_msg << std::endl;
        std::cout << "\tsize : " << send_size << std::endl;
    }
    return EXIT_SUCCESS;    
}
