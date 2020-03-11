#include <net/socket.h>
#include <net/sockaddr.h>

// UDP Client 
// 发送一个字符串到指定的服务器，服务器将返回字符串的长度
int main(int argc, char* argv[])
{
    using namespace o7si::net;
    // 默认发送至服务器的 10000 端口
    std::shared_ptr<UDPClientSocket> client_socket = 
        UDPClientSocket::GenSocket(SockAddrIn::LocalHost(10000));

    while (true)
    {
        // 目的端口以及消息
        int send_port;
        std::cout << "please input port: ";
        std::cin >> send_port;
        std::string send_msg;
        std::cout << "please input msg: ";
        std::cin >> send_msg;
    
        int send_size;
        // 发送至默认位置
        if (send_port == -1)
            send_size = client_socket->sendto(send_msg);
        else
        // 发送消息，向指定端口
            send_size = 
                client_socket->sendto(SockAddrIn::LocalHost(send_port), send_msg);

        // 如果消息发送失败
        if (send_size == -1)
            break; 

        std::cout << "send-info" << std::endl;
        std::cout << "\tmsg  : " << send_msg << std::endl;
        std::cout << "\tsize : " << send_size << std::endl;
        
        std::shared_ptr<SockAddr> from;
        std::string recv_msg;
        int recv_size = client_socket->recvfrom(from, recv_msg);

        // 如果消息接收失败
        if (recv_size == -1)
            break;

        std::cout << "recv-info" << std::endl;
        std::cout << "\tmsg  : " << recv_msg << std::endl;
        std::cout << "\tsize : " << recv_size << std::endl; 
        std::cout << "\tsour : " << std::endl;

        if (recv_msg == "quit")
            break;
    }
    return EXIT_SUCCESS;    
}
