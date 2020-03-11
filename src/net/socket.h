/****************************************************************************** * File: socket.h
 * Description: Socket 的封装。 
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <memory>

#include "sockaddr.h"


// | method   | success  | failure   |
// | socket   | fd       | -1, errno | 
// | bind     | 0        | -1, errno |  
// | listen   | 0        | -1, errno | 
// | accept   | fd       | -1, errno | 
// | connect  | 0        | -1, errno | 
// | recv     | received | -1, error |
// | send     | sent     | -1, errno | 
// | recvfrom | received | -1, errno |
// | sendto   | sent     | -1, errno |


namespace o7si
{
namespace net
{


// ---------------------------------------------------------------------------- 

// 服务端 TCP Socket
// socket -> bind -> listen -> accept -> recv/send -> close
class TCPServerSocket
{
public:
    // 创建 Socket 对象
    // 参数 sockaddr 表示服务端要绑定的地址
    // 参数 backlog 表示连接队列的最大长度
    static std::shared_ptr<TCPServerSocket> 
    GenSocket(std::shared_ptr<SockAddr> sockaddr, int backlog = 128);

    TCPServerSocket(std::shared_ptr<SockAddr> sockaddr);

    ~TCPServerSocket();

    // 建立一个 socket 并且生成文件描述符
    bool socket(); 

    // 绑定地址和端口 
    bool bind();

    // 进入被动监听状态
    bool listen(int backlog);

    // 服务端等待客户端的连接请求
    // 程序将进入阻塞状态
    // 返回值为一个整数，即文件描述符，标识一个客户端
    // 当返回值为负数时，表示出现错误
    int accept();

    // 向指定的客户端发送信息
    ssize_t send(int cli_fd, const void* buf, size_t len, 
                 int flags = 0);

    ssize_t send(int cli_fd, const std::string& buf,
                 int flags = 0);

    ssize_t write(int cli_fd, const void* buf, size_t len);

    ssize_t write(int cli_fd, const std::string& buf);
    
    // 从指定的客户端接收数据
    ssize_t recv(int cli_fd, void* buf, size_t len,
                 int flags = 0);

    ssize_t recv(int cli_fd, std::string& buf,
                 int flags = 0);

    ssize_t read(int cli_fd, void* buf, size_t len);

    ssize_t read(int cli_fd, std::string& buf);

    // 关闭服务端
    bool server_close();

    // 关闭客户端
    bool client_close(int cli_fd);

private:
    // 服务端 socket 对应的文件描述符
    int m_fd;   
    // 服务端 socket 绑定的地址信息
    std::shared_ptr<SockAddr> m_bind_sockaddr;
    // 目前正在连接的客户端对象
    std::unordered_map<int, std::shared_ptr<SockAddr>> m_cli_objs;
};

// ----------------------------------------------------------------------------

// 客户端 TCP Socket
// socket -> connet -> recv/send -> close
class TCPClientSocket
{
public:
    // 创建 Socket 对象
    // 参数 sockaddr 表示客户端请求的地址
    static std::shared_ptr<TCPClientSocket>
    GenSocket(std::shared_ptr<SockAddr> sockaddr);

    TCPClientSocket(std::shared_ptr<SockAddr> sockaddr);

    ~TCPClientSocket();

    // 建立一个 socket 并且生成文件描述符
    bool socket();

    // 客户端尝试连接服务器
    bool connect();

    // 向指定的客户端发送信息
    ssize_t send(const void* buf, size_t len, int flags = 0);

    ssize_t send(const std::string& buf, int flags = 0);

    ssize_t write(const void* buf, size_t len);

    ssize_t write(const std::string& buf);

    // 从指定的客户端接收数据
    ssize_t recv(void* buf, size_t len, int flags = 0);

    ssize_t recv(std::string& buf, int flags = 0);

    ssize_t read(void* buf, size_t len);

    ssize_t read(std::string& buf);

    // 关闭客户端
    bool client_close();

private:
    // 客户端 socket 对应的文件描述符
    int m_fd;
    // 客户端 socket 请求的地址信息
    std::shared_ptr<SockAddr> m_conn_sockaddr;
};
    
// ----------------------------------------------------------------------------

// 服务端 UDP Socket
// socket -> bind -> recvfrom/sendto -> close
class UDPServerSocket
{
public: 
    // 创建 Socket 对象
    // 参数 sockaddr 表示服务端要绑定的地址
    static std::shared_ptr<UDPServerSocket>
    GenSocket(std::shared_ptr<SockAddr> sockaddr); 
    
    UDPServerSocket(std::shared_ptr<SockAddr> sockaddr);

    ~UDPServerSocket();
      
    // 建立一个 socket 并且生成文件描述符
    bool socket(); 

    // 绑定地址和端口 
    bool bind();

    // 发送消息到某个客户端 
    // 当传入参数为 nullptr 时，将会导致段错误
    ssize_t sendto(const std::shared_ptr<SockAddr> to, 
                   const void* buf, size_t len, int flags = 0);

    ssize_t sendto(const std::shared_ptr<SockAddr> to,
                   const std::string& buf, int flags = 0);

    // 接收某个客户端的消息
    // 出现错误时，from 将会被置为 nullptr
    ssize_t recvfrom(std::shared_ptr<SockAddr>& from,
                     void* buf, size_t len, int flags = 0);

    ssize_t recvfrom(std::shared_ptr<SockAddr>& from,
                     std::string& buf, int flags = 0);

    // 关闭服务端
    bool server_close();

private:    
    // 服务端 socket 对应的文件描述符
    int m_fd;   
    // 服务端 socket 绑定的地址信息
    std::shared_ptr<SockAddr> m_bind_sockaddr;
};

// ----------------------------------------------------------------------------

// 客户端 UDP Socket
// socket -> recvfrom/sendto -> close
class UDPClientSocket
{
public:
    // 创建 Socket 对象
    // 参数 sockaddr 表示客户端发送消息的目的地址
    static std::shared_ptr<UDPClientSocket>
    GenSocket(std::shared_ptr<SockAddr> sockaddr); 

    UDPClientSocket(std::shared_ptr<SockAddr> sockaddr);

    ~UDPClientSocket();

    // 建立一个 socket 并且生成文件描述符
    bool socket(); 

    // 发送消息到指定的客户端
    ssize_t sendto(const std::shared_ptr<SockAddr> to, 
                   const void* buf, size_t len, int flags = 0);

    ssize_t sendto(const std::shared_ptr<SockAddr> to,
                   const std::string& buf, int flags = 0);

    // 发送消息到默认的客户端 
    ssize_t sendto(const void* buf, size_t len, int flags = 0);

    ssize_t sendto(const std::string& buf, int flags = 0);

    // 接收某个客户端的消息
    ssize_t recvfrom(std::shared_ptr<SockAddr>& from,
                     void* buf, size_t len, int flags = 0);

    ssize_t recvfrom(std::shared_ptr<SockAddr>& from,
                     std::string& buf, int flags = 0);

    // 关闭客户端
    bool client_close();

private:   
    // 客户端 socket 对应的文件描述符
    int m_fd;
    // 客户端 socket 发送消息的目的地址
    std::shared_ptr<SockAddr> m_dest_sockaddr;
};

// ----------------------------------------------------------------------------


}   // namespace net    
}   // namespace o7si
