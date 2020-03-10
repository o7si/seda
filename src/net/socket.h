/******************************************************************************
 * File: socket.h
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


// | method  | success  | failure   |
// | socket  | fd       | -1, errno | 
// | bind    | 0        | -1, errno |  
// | listen  | 0        | -1, errno | 
// | accept  | fd       | -1, errno | 
// | connect | 0        | -1, errno | 
// | recv    | received | -1, error |
// | send    | sent     | -1, errno | 


namespace o7si
{
namespace net
{


// ---------------------------------------------------------------------------- 

// 服务端 TCP Socket
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
    
    // 从指定的客户端接收数据
    ssize_t recv(int cli_fd, void* buf, size_t len,
                 int flags = 0);

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

    // 从指定的客户端接收数据
    ssize_t recv(void* buf, size_t len, int flags = 0);

    // 关闭客户端
    bool client_close();

private:
    // 客户端 socket 对应的文件描述符
    int m_fd;
    // 客户端 socket 请求的地址信息
    std::shared_ptr<SockAddr> m_conn_sockaddr;
};
    
// ----------------------------------------------------------------------------


}   // namespace net    
}   // namespace o7si
