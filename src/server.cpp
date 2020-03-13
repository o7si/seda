#include "server.h"


namespace o7si
{
namespace server
{


// ----------------------------------------------------------------------------


WebServer::WebServer(int protocol, int port, std::string path)
    : m_port(port), m_path(std::move(path)),
      m_pool_capacity(5)
{
    setProtocol(protocol);
}

WebServer::~WebServer()
{
    stop(); 
}

bool WebServer::start()
{
    m_shutdown = false;
    // 建立 Socket，进入监听状态，等待客户端的连接
    if (m_protocol == 4)
    {
        m_socket = o7si::net::TCPServerSocket::GenSocket(
            o7si::net::SockAddrIn::LocalHost(m_port)        
        ); 
    }
    else if (m_protocol == 6)
    {
        m_socket = o7si::net::TCPServerSocket::GenSocket(
            o7si::net::SockAddrIn6::LocalHost(m_port)
        );    
    }

    // 建立 Socket 失败
    if (m_socket == nullptr)
    {
        LOG_WARN_SYS << "webserver start failure."; 
        m_shutdown = true;
        return false;     
    }

    // 设定超时时间
    m_socket->setSendTimeout(1);
    m_socket->setRecvTimeout(1);

    // 启动线程
    for (int i = 0; i < m_pool_capacity; ++ i)
        m_worker_pool.emplace_back(Worker(this, i));

    LOG_INFO_SYS << "webserver start success. "
                 << "port = " << m_port << ", "
                 << "protocol = " << m_protocol << ", "
                 << "path = " << m_path;
    return true;
}

bool WebServer::restart()
{
    // 停止服务
    if (!stop())
        return false;
    // 启动服务
    if (!start())
        return false;

    LOG_INFO_SYS << "webserver restart success.";
    return true;    
}

bool WebServer::stop()
{
    // 当服务未被关闭
    if (!m_shutdown)
    {
        // 调整服务状态 
        m_shutdown = true;   
        // 等待线程服务完毕
        for (auto& thread : m_worker_pool)
            thread.join();
        // 清理 Socket 资源
        m_socket->close(); 
        m_socket = nullptr;
    }

    LOG_INFO_SYS << "webserver stop success.";
    return true;
}

WebServer::Worker::Worker(WebServer* server, int id)
    : m_server(server), m_id(id)
{
}

void WebServer::Worker::operator()()
{
    // 如果服务未关闭，则循环等待客户端的连接
    while (!m_server->m_shutdown)
    {
        int cli_fd = m_server->m_socket->accept();   
        // 客户端连接到服务器时出现错误
        if (cli_fd == -1)
            continue;
        LOG_DEBUG_SYS << "connection establishment" 
                      << "(" << cli_fd << ")";

        // 如果服务未关闭，则循环处理客户端的请求
        while (!m_server->m_shutdown)
        {
            // 客户端的 HTTP 请求报文
            std::string req_msg;
            ssize_t recv_size = m_server->m_socket->recv(cli_fd, req_msg); 

            // 当 size 等于 0 时，表示客户端主动断开了连接
            // 当 size 等于 -1 时，表示接收数据时发生了错误
            if (recv_size == 0 || recv_size == -1)
            {
                m_server->m_socket->client_close(cli_fd); 
                break;
            }

            // 成功接收到客户端的 HTTP 请求报文
            // 开始解析其内容
            o7si::net::HttpRequestParser req_parser;
            req_parser.parser_execute(req_msg.data(), req_msg.size());

            // 解析结果
            std::shared_ptr<o7si::net::HttpRequest> request 
                = req_parser.inner_request();

            // 处理 HTTP 请求
            o7si::net::Servlet servlet(m_server->m_path);
            std::shared_ptr<o7si::net::HttpResponse> response
                = std::make_shared<o7si::net::HttpResponse>();
            servlet.todo(request, response);

            // 发送 HTTP 响应报文到客户端
            std::string res_msg = response->format();
            m_server->m_socket->send(cli_fd, res_msg);
        }
        LOG_DEBUG_SYS << "disconnect" 
                      << "(" << cli_fd << ")";
    }
}

// ----------------------------------------------------------------------------

 
}   // namespace server    
}   // namespace o7si
