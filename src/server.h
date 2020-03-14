/******************************************************************************
 * File: server.h
 * Description: 提供对外接口，调节相关细节。 
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <stdexcept>
#include <string>
#include <memory>
#include <thread>

#include "log.h"

#include "net/http.h"
#include "net/http_parser.h"
#include "net/sockaddr.h"
#include "net/socket.h"
#include "net/servlet.h"


namespace o7si
{    
namespace server
{
    

// ----------------------------------------------------------------------------

// 监听端口，提供 Web 服务，调节服务器的内部细节
// 修改某些参数时，需要重启服务才可生效
class WebServer
{
public:
    // protocol
    //      4: 使用 IPv4 地址
    //      6：使用 IPv6 地址
    //   其它：使用 IPv4 地址 
    // port: 服务器监听的端口
    // path: 网页的路径
    WebServer(int protocol, int port, std::string path);

    // 清理资源
    ~WebServer();

    int getProtocol() const 
    {
        return m_protocol; 
    }

    void setProtocol(int protocol) 
    {
        // 协议指定不合法时，默认使用 IPv4 协议
        if (protocol != 4 && protocol != 6)
        {
            m_protocol = 4;
            return;
        }
        m_protocol = protocol; 
    }

    int getPort() const 
    { 
        return m_port; 
    }

    void setPort(int port) 
    { 
        m_port = port;
    }

    std::string getPath() const
    {
        return m_path;    
    }

    void setPath(std::string path)
    {
        m_path = std::move(path);    
    }

    int getThreadPoolCapacity() const
    {
        return m_pool_capacity;   
    }

    void setThreadPoolCapacity(int capacity)
    {
        m_pool_capacity = capacity;    
    }

    // 启动服务
    // 当服务处于关闭状态时，则等同于真正意义上的启动
    // 当服务处于启动状态时，则什么都不做
    bool start();

    // 重启服务
    // 当服务处于关闭状态时，则等同于启动服务
    // 当服务处于启动状态时，则等同于真正意义上的重启
    bool restart();

    // 停止服务
    // 当服务处于关闭状态时，则什么都不做
    // 当服务处于启动状态时，则等同于真正意义上的关闭
    bool stop();

private:  
    // 协议（需要重启服务）
    int m_protocol;
    // 端口号（需要重启服务）
    int m_port;
    // 网页路径（不需要重启服务）
    std::string m_path;

    // 服务关闭标志
    bool m_shutdown;

    // TCP Socket
    std::shared_ptr<o7si::net::TCPServerSocket> m_socket;

    // 工作线程池
    // 由于该服务的存在意识是为了监控和调节服务器的内部细节
    // 所以无法调整该线程池的容量
    std::vector<std::thread> m_worker_pool;
    // 线程池容量
    int m_pool_capacity;
    
    // 线程任务
    class Worker
    {
    public:
        Worker(WebServer* server, int id);   
        
        void operator()();

    private:
        WebServer* m_server;
        int m_id;   
    };
};

// ----------------------------------------------------------------------------


}   // namespace server
}   // namespace o7si
