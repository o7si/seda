/******************************************************************************
 * File: web_server.h
 * Description: 提供 Web 服务，从而调整框架的内部细节。 
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include "../log.h"
#include "../net/http.h"
#include "../net/http_parser.h"
#include "../net/sockaddr.h"
#include "../net/socket.h"
#include "../random.h"

#include "servlet.h"


namespace o7si
{    
namespace server
{
    
// ----------------------------------------------------------------------------

class WebServerManager
{
public:
    // 获取验证码
    std::string getAuthCode() const
    {
        return m_auth_code;    
    }

    // 设置验证码（不推荐）
    void setAuthCode(std::string code)
    {
        m_auth_code = std::move(code);    
    }

    // 生成验证码
    void genAuthCode()
    {
        m_auth_code = o7si::random::uuid();    
    }

    // 获取存储路径
    std::string getAuthPath() const
    {
        return m_auth_path;        
    }

    // 设置存储路径
    void setAuthPath(std::string path)
    {
        m_auth_path = std::move(path);    
    }
    
    void isAuth(bool flag)
    {
        is_auth = flag; 
    } 

    // 是否进行验证
    bool isAuth() const
    {
        return is_auth;    
    }

    void isSave(bool flag)
    {
        is_save = flag;    
    }

    // 是否进行存储
    bool isSave() const
    {
        return is_save;    
    }

    // 存储
    void save();

public:
    // 获取单例对象
    static std::shared_ptr<WebServerManager> Instance();

    // 删除拷贝构造函数，防止用户拷贝对象
    WebServerManager(const WebServerManager& other) = delete;

    // 删除移动构造函数，防止用户使用移动语义
    WebServerManager(WebServerManager&& other) = delete;

private:
    // 将构造函数修饰为私有，防止用户擅自实例化对象
    WebServerManager();
    
    // 身份验证
    std::string m_auth_code;  
    // 存储路径
    std::string m_auth_path;

    // 是否进行验证
    bool is_auth;
    // 是否进行存储
    bool is_save;
};

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
