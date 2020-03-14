#include "socket.h" 


namespace o7si
{
namespace net
{
    
// ----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& stream,
                         const Socket::Timeout& rhs)
{
    return stream << "["
                  << rhs.m_seconds << "s "
                  << rhs.m_milliseconds << "ms "
                  << rhs.m_microseconds << "us"  
                  << "]";
}

Socket::Timeout::Timeout(int s, int ms, int us)
    : m_seconds(s), m_milliseconds(ms), m_microseconds(us)
{
}

Socket::Timeout Socket::getSendTimeout() const
{
    return m_send_timeout;    
}

Socket::Socket()
    : m_log(true)
{
}

void Socket::setSendTimeout(const Socket::Timeout& timeout)
{
    timeval tv;
    tv.tv_sec = timeout.s();
    tv.tv_usec = timeout.ms() * 1000 + timeout.us();    
    
    int ret = ::setsockopt(m_fd, SOL_SOCKET, SO_SNDTIMEO, 
                           &tv, sizeof(tv));
    if (ret == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "set send timeout failure, "
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ". "
            << "timeout = " << timeout;
        return;
    }
    m_send_timeout = timeout;
}

void Socket::setSendTimeout(int s, int ms, int us)
{
    setSendTimeout(Socket::Timeout(s, ms, us));    
}

Socket::Timeout Socket::getRecvTimeout() const
{
    return m_recv_timeout;    
}

void Socket::setRecvTimeout(const Socket::Timeout& timeout)
{
    timeval tv;
    tv.tv_sec = timeout.s();
    tv.tv_usec = timeout.ms() * 1000 + timeout.us();    

    int ret = ::setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO, 
                           &tv, sizeof(tv));
    if (ret == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "set recv timeout failure, "
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ". "
            << "timeout = " << timeout;
        return;
    }
    m_recv_timeout = timeout; 
}

void Socket::setRecvTimeout(int s, int ms, int us)
{
    setRecvTimeout(Socket::Timeout(s, ms, us));
}

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
    close();
}

bool TCPServerSocket::socket()
{
    int ret = ::socket(m_bind_sockaddr->get_c_data()->sa_family, 
                       SOCK_STREAM, 
                       0);
    if (ret == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "socket failure, "
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
        LOG_WARN_CHECK_SYS(m_log) 
            << "bind failure, "
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
        LOG_WARN_CHECK_SYS(m_log) 
            << "bind failure, "
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
        LOG_WARN_CHECK_SYS(m_log) 
            << "accept failure, "
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
            LOG_WARN_CHECK_SYS(m_log) 
                << "accept failure, "
                << "unknown family.";
            return -1;
    }

    if (m_cli_objs.find(ret) != m_cli_objs.end())
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "accept failure, "
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
        LOG_WARN_CHECK_SYS(m_log) 
            << "send failure, "
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

ssize_t TCPServerSocket::write(int cli_fd, const void* buf, size_t len)
{
    return send(cli_fd, buf, len, 0);    
}

ssize_t TCPServerSocket::write(int cli_fd, const std::string& buf)
{
    return send(cli_fd, buf, 0);    
}

ssize_t TCPServerSocket::recv(int cli_fd, void* buf, size_t len,
                              int flags)
{
    int received = ::recv(cli_fd, buf, len, flags);
    if (received == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "recv failure, "
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

ssize_t TCPServerSocket::read(int cli_fd, void* buf, size_t len)
{
    return recv(cli_fd, buf, len, 0);    
}

ssize_t TCPServerSocket::read(int cli_fd, std::string& buf)
{
    return recv(cli_fd, buf, 0);    
}

bool TCPServerSocket::close()
{
    int count = 0;
    for (const auto& item : m_cli_objs)
        if (!client_close(item.first))
            ++ count;

    if (!server_close())
        ++ count;

    return count == 0;
}

bool TCPServerSocket::server_close()
{
    int ret = ::close(m_fd); 
    if (ret == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "close failure, "    
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
        LOG_WARN_CHECK_SYS(m_log)
            << "close failure, "    
            << cli_fd << "(fd) not exist.";     
        return false;
    }

    int ret = ::close(cli_fd);
    if (ret == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "close failure, "    
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
    close();
}

bool TCPClientSocket::socket()
{
    int ret = ::socket(m_conn_sockaddr->get_c_data()->sa_family, 
                       SOCK_STREAM, 
                       0);
    if (ret == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "socket failure, "
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
        LOG_WARN_CHECK_SYS(m_log) 
            << "connect failure, "
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
        LOG_WARN_CHECK_SYS(m_log) 
            << "send failure, "
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ".";
    }
    return sent;
}

ssize_t TCPClientSocket::send(const std::string& buf, int flags)
{
    return send(buf.data(), buf.size(), flags);     
}

ssize_t TCPClientSocket::write(const void* buf, size_t len)
{
    return send(buf, len, 0);    
}

ssize_t TCPClientSocket::write(const std::string& buf)
{
    return send(buf, 0);    
}

ssize_t TCPClientSocket::recv(void* buf, size_t len, int flags)
{
    int received = ::recv(m_fd, buf, len, flags);
    if (received == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "recv failure, "
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

ssize_t TCPClientSocket::read(void* buf, size_t len)
{
    return recv(buf, len, 0);    
}

ssize_t TCPClientSocket::read(std::string& buf)
{
    return recv(buf, 0);    
}

bool TCPClientSocket::close()
{
    return client_close();
}

bool TCPClientSocket::client_close()
{
    int ret = ::close(m_fd);
    if (ret == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "close failure, "
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ".";
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------

std::shared_ptr<UDPServerSocket> 
UDPServerSocket::GenSocket(std::shared_ptr<SockAddr> sockaddr)
{
    auto server_socket = std::make_shared<UDPServerSocket>(sockaddr);

    if (!server_socket->socket())
        return nullptr;

    if (!server_socket->bind())
        return nullptr;

    return server_socket;
}

UDPServerSocket::UDPServerSocket(std::shared_ptr<SockAddr> sockaddr)
    : m_bind_sockaddr(sockaddr) 
{    
}

UDPServerSocket::~UDPServerSocket()
{
    close();
}

bool UDPServerSocket::socket()
{
    int ret = ::socket(m_bind_sockaddr->get_c_data()->sa_family, 
                       SOCK_DGRAM, 
                       0);
    if (ret == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "socket failure, "
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ".";
        return false;
    }
    m_fd = ret;     
    return true;
}

bool UDPServerSocket::bind()
{
    int ret = ::bind(m_fd, 
                     m_bind_sockaddr->get_c_data(),
                     m_bind_sockaddr->length());    
    if (ret == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "bind failure, "
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ".";
        return false;
    }
    return true;
}

ssize_t UDPServerSocket::sendto(const std::shared_ptr<SockAddr> to,
                                const void* buf, size_t len, int flags)
{
    int sent = ::sendto(m_fd, buf, len, flags, 
                        to->get_c_data(), to->length());     
    if (sent == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "send failure, "
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ".";
    }
    return sent;
}

ssize_t UDPServerSocket::sendto(const std::shared_ptr<SockAddr> to,
                                const std::string& buf, int flags)
{
    return sendto(to, buf.data(), buf.size(), flags);
}

ssize_t UDPServerSocket::recvfrom(std::shared_ptr<SockAddr>& from,
                                  void* buf, size_t len, int flags)
{
    sockaddr_storage storage;
    socklen_t storage_len = sizeof(storage);

    int received = ::recvfrom(m_fd, buf, len, flags, 
                              (sockaddr*)&storage, &storage_len); 
    if (received == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "recvfrom failure, "
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ".";
        from = nullptr;
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
            LOG_WARN_CHECK_SYS(m_log) 
                << "recvfrom failure, "
                << "unknown family.";
            from = nullptr;
            return -1;
    }

    from = cli_sockaddr;
    return received;
}

ssize_t UDPServerSocket::recvfrom(std::shared_ptr<SockAddr>& from,
                                  std::string& buf, int flags)
{
    char tmp[BUFSIZ];
    memset(tmp, 0, sizeof(tmp));

    ssize_t received = recvfrom(from, tmp, sizeof(tmp), flags);
    buf = received == -1 ? "" : std::string(tmp, received);
    return received;
}

bool UDPServerSocket::close()
{
    return server_close();
}

bool UDPServerSocket::server_close()
{
    int ret = ::close(m_fd); 
    if (ret == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "close failure, "    
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ".";
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------

std::shared_ptr<UDPClientSocket>
UDPClientSocket::GenSocket(std::shared_ptr<SockAddr> sockaddr)
{
    auto client_socket = std::make_shared<UDPClientSocket>(sockaddr);

    if (!client_socket->socket())
        return nullptr;

    return client_socket;
}

UDPClientSocket::UDPClientSocket(std::shared_ptr<SockAddr> sockaddr)
    : m_dest_sockaddr(sockaddr)
{
}

UDPClientSocket::~UDPClientSocket()
{
    close();
}

bool UDPClientSocket::socket()
{
    int ret = ::socket(m_dest_sockaddr->get_c_data()->sa_family, 
                       SOCK_DGRAM,
                       0);
    if (ret == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "socket failure, "
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ".";
        return false;
    }
    m_fd = ret;     
    return true;
}

ssize_t UDPClientSocket::sendto(const std::shared_ptr<SockAddr> to,
                                const void* buf, size_t len, int flags)
{
    int sent = ::sendto(m_fd, buf, len, flags,
                        to->get_c_data(),
                        to->length());   

    if (sent == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "send failure, "
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ".";
    }
    return sent;
}

ssize_t UDPClientSocket::sendto(const std::shared_ptr<SockAddr> to,
                                const std::string& buf, int flags)
{
    return sendto(to, buf.data(), buf.size(), flags);    
}

ssize_t UDPClientSocket::sendto(const void* buf, size_t len, int flags)
{
    return sendto(m_dest_sockaddr, buf, len, flags); 
}

ssize_t UDPClientSocket::sendto(const std::string& buf, int flags)
{
    return sendto(m_dest_sockaddr, buf, flags);    
}

ssize_t UDPClientSocket::recvfrom(std::shared_ptr<SockAddr>& from,
                                  void* buf, size_t len, int flags)
{
    sockaddr_storage storage;
    socklen_t storage_len = sizeof(storage);

    int received = ::recvfrom(m_fd, buf, len, flags, 
                              (sockaddr*)&storage, &storage_len); 
    if (received == -1)
    {
        LOG_WARN_CHECK_SYS(m_log) 
            << "recvfrom failure, "
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ".";
        return -1;
    }

    std::shared_ptr<SockAddr> ser_sockaddr;
    switch (storage.ss_family)
    {
        case AF_INET:
            ser_sockaddr = std::make_shared<SockAddrIn>(&storage);
            break;
        case AF_INET6:
            ser_sockaddr = std::make_shared<SockAddrIn6>(&storage);
            break;
        case AF_UNIX:
            ser_sockaddr = std::make_shared<SockAddrUn>(&storage);
            break;
        default:   
            LOG_WARN_CHECK_SYS(m_log) 
                << "recvfrom failure, "
                << "unknown family.";
            return -1;
    }

    from = ser_sockaddr;
    return received;
}

ssize_t UDPClientSocket::recvfrom(std::shared_ptr<SockAddr>& from,
                                  std::string& buf, int flags)
{
    char tmp[BUFSIZ];
    memset(tmp, 0, sizeof(tmp));

    ssize_t received = recvfrom(from, tmp, sizeof(tmp), flags);
    buf = received == -1 ? "" : std::string(tmp, received);
    return received;
}

bool UDPClientSocket::close()
{
    return client_close();
}

bool UDPClientSocket::client_close()
{
    int ret = ::close(m_fd); 
    if (ret == -1)
    {
        LOG_WARN_CHECK_SYS(m_log)
            << "close failure, "    
            << "error desc is '" << strerror(errno) << "', "
            << "error code is " << errno << ".";
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------

}   // namespace net    
}   // namespace o7si
