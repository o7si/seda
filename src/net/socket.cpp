#include "socket.h"


namespace o7si
{
namespace net
{
    

// ----------------------------------------------------------------------------

std::shared_ptr<TCPServerSocket> 
TCPServerSocket::GenSocket(std::shared_ptr<SockAddr> sockaddr, int backlog)
{   
    auto server_socket = std::make_shared<TCPServerSocket>(sockaddr);
    
    if (!server_socket->socket())
        return nullptr;

    if (!server_socket->bind())
        return nullptr;

    if (!server_socket->listen(backlog))
        return nullptr;

    return server_socket;
}

TCPServerSocket::TCPServerSocket(std::shared_ptr<SockAddr> sockaddr)
    : m_bind_sockaddr(sockaddr)
{
}

TCPServerSocket::~TCPServerSocket()
{
    for (const auto& item : m_cli_objs)
        client_close(item.first);
    server_close();
}

bool TCPServerSocket::socket()
{
    int ret = ::socket(m_bind_sockaddr->get_c_data()->sa_family, 
                       SOCK_STREAM, 
                       0);
    if (ret == -1)
    {
        LOG_WARN_SYS << "socket failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
        return false;
    }
    m_fd = ret;     
    return true;
}

bool TCPServerSocket::bind()
{
    int ret = ::bind(m_fd, 
                     m_bind_sockaddr->get_c_data(),
                     m_bind_sockaddr->length());    
    if (ret == -1)
    {
        LOG_WARN_SYS << "bind failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
        return false;
    }
    return true;
}

bool TCPServerSocket::listen(int backlog)
{
    int ret = ::listen(m_fd, backlog);
    if (ret == -1)
    {
        LOG_WARN_SYS << "bind failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
        return false;
    }
    return true;    
}

int TCPServerSocket::accept()
{
    sockaddr_storage storage;
    socklen_t storage_len = sizeof(storage);
    
    int ret = ::accept(m_fd, (sockaddr*)&storage, &storage_len);     
    if (ret == -1)
    {
        LOG_WARN_SYS << "accept failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
        return -1;
    }

    std::shared_ptr<SockAddr> cli_sockaddr;
    switch (storage.ss_family)
    {
        case AF_INET:
            cli_sockaddr = std::make_shared<SockAddrIn>(&storage);
            break;
        case AF_INET6:
            cli_sockaddr = std::make_shared<SockAddrIn6>(&storage);
            break;
        case AF_UNIX:
            cli_sockaddr = std::make_shared<SockAddrUn>(&storage);
            break;
        default:   
            LOG_WARN_SYS << "accept failure, "
                         << "unknown family.";
            return -1;
    }

    if (m_cli_objs.find(ret) != m_cli_objs.end())
    {
        LOG_WARN_SYS << "accept failure, "
                     << ret << "(fd) already exists.";     
        return -1;
    }

    m_cli_objs[ret] = cli_sockaddr;
    return ret;
}

ssize_t TCPServerSocket::send(int cli_fd, const void* buf, size_t len, 
                              int flags)
{
    int sent = ::send(cli_fd, buf, len, flags);    
    if (sent == -1)
    {
        LOG_WARN_SYS << "send failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
    }
    return sent;
}

ssize_t TCPServerSocket::send(int cli_fd, const std::string& buf,
                              int flags)
{
    return send(cli_fd, buf.data(), buf.size(), flags);    
}

ssize_t TCPServerSocket::recv(int cli_fd, void* buf, size_t len,
                              int flags)
{
    int received = ::recv(cli_fd, buf, len, flags);
    if (received == -1)
    {
        LOG_WARN_SYS << "recv failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
    }
    return received;
}

ssize_t TCPServerSocket::recv(int cli_fd, std::string& buf, 
                              int flags)
{
    char tmp[BUFSIZ];
    memset(tmp, 0, sizeof(tmp));
     
    ssize_t received = recv(cli_fd, tmp, sizeof(tmp), flags); 
    buf = received == -1 ? "" : std::string(tmp, received);
    return received;
}

bool TCPServerSocket::server_close()
{
    int ret = ::close(m_fd); 
    if (ret == -1)
    {
        LOG_WARN_SYS << "close failure, "    
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
        return false;
    }
    return true;
}

bool TCPServerSocket::client_close(int cli_fd)
{
    auto iter = m_cli_objs.find(cli_fd);
    if (iter == m_cli_objs.end())    
    {
        LOG_WARN_SYS << "close failure, "    
                     << cli_fd << "(fd) not exist.";     
        return false;
    }

    int ret = ::close(cli_fd);
    if (ret == -1)
    {
        LOG_WARN_SYS << "close failure, "    
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
        return false;
    }

    m_cli_objs.erase(iter); 
    return true;
}

// ----------------------------------------------------------------------------

std::shared_ptr<TCPClientSocket> 
TCPClientSocket::GenSocket(std::shared_ptr<SockAddr> sockaddr)
{
    auto client_socket = std::make_shared<TCPClientSocket>(sockaddr);   
    if (!client_socket->socket())
        return nullptr;

    return client_socket;
}

TCPClientSocket::TCPClientSocket(std::shared_ptr<SockAddr> sockaddr)
    : m_conn_sockaddr(sockaddr)
{
} 

TCPClientSocket::~TCPClientSocket()
{
    client_close();
}

bool TCPClientSocket::socket()
{
    int ret = ::socket(m_conn_sockaddr->get_c_data()->sa_family, 
                       SOCK_STREAM, 
                       0);
    if (ret == -1)
    {
        LOG_WARN_SYS << "socket failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
        return false;
    }
    m_fd = ret;     
    return true;
}

bool TCPClientSocket::connect()
{
    int ret = ::connect(m_fd,
                        m_conn_sockaddr->get_c_data(),
                        m_conn_sockaddr->length());      
    if (ret == -1)
    {
        LOG_WARN_SYS << "connect failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
        return false;
    }
    return true;
}

ssize_t TCPClientSocket::send(const void* buf, size_t len, int flags)
{
    int sent = ::send(m_fd, buf, len, flags);    
    if (sent == -1)
    {
        LOG_WARN_SYS << "send failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
    }
    return sent;
}

ssize_t TCPClientSocket::send(const std::string& buf, int flags)
{
    return send(buf.data(), buf.size(), flags);     
}

ssize_t TCPClientSocket::recv(void* buf, size_t len, int flags)
{
    int received = ::recv(m_fd, buf, len, flags);
    if (received == -1)
    {
        LOG_WARN_SYS << "recv failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
    }
    return received;
}

ssize_t TCPClientSocket::recv(std::string& buf, int flags)
{
    char tmp[BUFSIZ];
    memset(tmp, 0, sizeof(tmp));
    
    ssize_t received = recv(tmp, sizeof(tmp), flags);
    buf = received == -1 ? "" : std::string(tmp, received);
    return received;     
}

bool TCPClientSocket::client_close()
{
    int ret = ::close(m_fd);
    if (ret == -1)
    {
        LOG_WARN_SYS << "close failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is " << errno << ".";
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------


}   // namespace net    
}   // namespace o7si
