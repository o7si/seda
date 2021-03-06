#include "sockaddr.h" 

namespace o7si
{
namespace net
{

// ----------------------------------------------------------------------------

Lookup::HostInfo::HostInfo(std::string name, std::string addr)
    : m_name(std::move(name)), m_addr(std::move(addr))
{
}

void Lookup::lookup(const std::string& host)
{
    addrinfo hints;
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_protocol = IPPROTO_IP;

    addrinfo* res;
    // zero     成功
    // non-zero 错误码 
    int errcode = getaddrinfo(host.data(), nullptr, &hints, &res); 
    if (errcode != 0)
    {
        LOG_WARN_SYS << "lookup failure, "
                     << "error desc is '" << gai_strerror(errcode) << "', "
                     << "error code is " << errcode << ". "
                     << "host = " << host << ".";
        return;    
    }

    char ipv4_buffer[INET_ADDRSTRLEN];
    memset(ipv4_buffer, 0, sizeof(ipv4_buffer));
    char ipv6_buffer[INET6_ADDRSTRLEN];
    memset(ipv6_buffer, 0, sizeof(ipv6_buffer));

    for (addrinfo* ptr = res; ptr != nullptr; ptr = ptr->ai_next)
    {
        std::string hostname = ptr->ai_canonname != nullptr ? 
                               ptr->ai_canonname : 
                               "null"; 

        switch (ptr->ai_family)
        {
            // IPv4
            case AF_INET:
            {
                // null     转换成功
                // non-null 转换失败 
                const char* addr = inet_ntop(AF_INET, 
                                             &((sockaddr_in*)ptr->ai_addr)->sin_addr,
                                             ipv4_buffer, 
                                             sizeof(ipv4_buffer));
                if (addr == nullptr)
                {
                    LOG_WARN_SYS << "address(IPv4) conver failure, "
                                 << "error desc is '" << strerror(errno) << "', " 
                                 << "error code is " << errno << ".";
                }

                m_ipv4.emplace_back(HostInfo(hostname, addr)); 
                break;
            }
            // IPv6
            case AF_INET6:
            {
                // null     转换成功
                // non-null 转换失败 
                const char* addr = inet_ntop(AF_INET6,
                                             &((sockaddr_in6*)ptr->ai_addr)->sin6_addr,
                                             ipv6_buffer,
                                             sizeof(ipv6_buffer));    
                if (addr == nullptr)
                {
                    LOG_WARN_SYS << "address(IPv6) conver failure, "
                                 << "error desc is '" << strerror(errno) << "', " 
                                 << "error code is " << errno << ".";
                }

                m_ipv6.emplace_back(HostInfo(hostname, addr)); 
                break;
            }
            // Other
            default:
            {
                break;          
            }
        }
    }
    freeaddrinfo(res);
}

// ----------------------------------------------------------------------------

SockAddr::SockAddr()
    : m_desc("no desc.")
{
}

// ----------------------------------------------------------------------------

// SockAddrInet

// ----------------------------------------------------------------------------

std::shared_ptr<SockAddrIn> SockAddrIn::LocalHost(uint32_t port)
{
    return std::make_shared<SockAddrIn>("0.0.0.0", port);    
}

std::shared_ptr<SockAddrIn> SockAddrIn::LocalHost(const std::string& port)
{
    return SockAddrIn::LocalHost(std::stoi(port));    
}

SockAddrIn::SockAddrIn()
{
    enable_work(false);
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));    
    m_sockaddr.sin_family = AF_INET;
}

SockAddrIn::SockAddrIn(const sockaddr* value)
{
    enable_work(true);
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));    
    set_c_data(value);
}

SockAddrIn::SockAddrIn(const sockaddr_in* value)
    : SockAddrIn((const sockaddr*)value) 
{    
}

SockAddrIn::SockAddrIn(const sockaddr_storage* value)
    : SockAddrIn((const sockaddr*)value)
{
}

SockAddrIn::SockAddrIn(const std::string& addr, uint32_t port)
{
    enable_work(false);
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));    
    m_sockaddr.sin_family = AF_INET;
    setAddr(addr);
    setPort(port);
}

SockAddrIn::SockAddrIn(const std::string& addr, const std::string& port)
    : SockAddrIn(addr, std::stoi(port))
{
}

std::string SockAddrIn::getAddr() const
{
    std::string def = "0.0.0.0";

    if (!m_addr_available)
    {
        LOG_WARN_SYS << "address invalid, "
                     << "return '" << def << "'.";
        return def;
    }

    char buffer[INET_ADDRSTRLEN];
    memset(buffer, 0, sizeof(buffer));
    const char* addr = inet_ntop(AF_INET, &m_sockaddr.sin_addr, 
                                 buffer, sizeof(buffer));
    if (addr == nullptr)
    {
        LOG_WARN_SYS << "address get failure, "
                     << "error desc is '" << strerror(errno) << "', " 
                     << "error code is " << errno << ", "
                     << "return '" << def << "'.";  
        return def;
    }
    return addr;
}

void SockAddrIn::setAddr(const std::string& addr)
{
    // -1   不合法的协议族 
    // 0    转换失败
    // 1    转换成功
    int ret = inet_pton(AF_INET, 
            addr.data(), 
            &m_sockaddr.sin_addr.s_addr);    

    if (ret == -1)
    {
        LOG_WARN_SYS << "address set failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is '" << errno << ". "
                     << "addr = " << addr << ".";
        m_addr_available = false;
        return;
    }

    if (ret == 0)
    {
        LOG_WARN_SYS << "address set failure. " 
                     << "addr = " << addr << ".";
        m_addr_available = false;
        return;
    }
    m_addr_available = true;
}

uint32_t SockAddrIn::getPort() const
{
    uint32_t def = 0;

    if (!m_port_available)
    {
        LOG_WARN_SYS << "port invalid, "
                     << "return " << def << ".";    
        return def;
    }
    // 返回主机字节序
    return ntohs(m_sockaddr.sin_port);    
}

void SockAddrIn::setPort(uint32_t port)
{
    if (port < 0 || port > 65535)
    {
        LOG_WARN_SYS << "port set failure. "
                     << "port = " << port; 
        m_port_available = false;
        return;    
    }
    // 设置成网络字节序
    m_sockaddr.sin_port = htons(port);    
    m_port_available = true;
}

void SockAddrIn::setPort(const std::string& port)
{
    setPort(std::stoi(port));    
}

sockaddr* SockAddrIn::get_c_data()
{
    if (!is_work())
        LOG_WARN_SYS << "SockAddrIn may not work.";    
    return (sockaddr*)&m_sockaddr;    
}

const sockaddr* SockAddrIn::get_c_data() const
{
    if (!is_work())
        LOG_WARN_SYS << "SockAddrIn may not work.";    
    return (const sockaddr*)&m_sockaddr;    
}

void SockAddrIn::set_c_data(const sockaddr* sockaddr)
{
    if (sockaddr->sa_family != AF_INET)
    {
        LOG_WARN_SYS << "c_data set failure, "
                     << "family must be AF_INET.";
        enable_work(false);
        return;
    } 
    
    m_sockaddr = *(sockaddr_in*)sockaddr;
    enable_work(true);
}

socklen_t SockAddrIn::length() const
{
    if (!is_work())
        LOG_WARN_SYS << "SockAddrIn may not work.";    
    return sizeof(m_sockaddr);    
}

std::string SockAddrIn::format() const
{
    std::stringstream stream;
    stream << "[IPv4]"
           << "[" << getAddr() << "]"   
           << ":" << getPort()
           << "(" << (is_work() ? "work" : "not work") << ")"
           << "|" << getDesc();
    return stream.str();
}

// ----------------------------------------------------------------------------

std::shared_ptr<SockAddrIn6> SockAddrIn6::LocalHost(uint32_t port)
{
    return std::make_shared<SockAddrIn6>("::", port);    
}

std::shared_ptr<SockAddrIn6> SockAddrIn6::LocalHost(const std::string& port)
{
    return SockAddrIn6::LocalHost(std::stoi(port));    
}

SockAddrIn6::SockAddrIn6()
{
    enable_work(false);
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));    
    m_sockaddr.sin6_family = AF_INET6;
}

SockAddrIn6::SockAddrIn6(const sockaddr* value)
{
    enable_work(true);
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));    
    set_c_data(value);
}

SockAddrIn6::SockAddrIn6(const sockaddr_in* value)
    : SockAddrIn6((const sockaddr*)value) 
{    
}

SockAddrIn6::SockAddrIn6(const sockaddr_storage* value)
    : SockAddrIn6((const sockaddr*)value)
{
}

SockAddrIn6::SockAddrIn6(const std::string& addr, uint32_t port)
{
    enable_work(false);
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));    
    m_sockaddr.sin6_family = AF_INET6;
    setAddr(addr);
    setPort(port);
}

SockAddrIn6::SockAddrIn6(const std::string& addr, const std::string& port)
    : SockAddrIn6(addr, std::stoi(port))
{    
}

std::string SockAddrIn6::getAddr() const
{
    std::string def = "::";

    if (!m_addr_available)
    {
        LOG_WARN_SYS << "address invalid, "
                     << "return '" << def << "'.";
        return def;
    }

    char buffer[INET6_ADDRSTRLEN];
    memset(buffer, 0, sizeof(buffer));
    const char* addr = inet_ntop(AF_INET6, &m_sockaddr.sin6_addr, 
                                 buffer, sizeof(buffer));
    if (addr == nullptr)
    {
        LOG_WARN_SYS << "address get failure, "
                     << "error desc is '" << strerror(errno) << "', " 
                     << "error code is " << errno << ", "
                     << "return '" << def << "'.";  
        return def;
    }
    return addr;
}

void SockAddrIn6::setAddr(const std::string& addr)
{
    // -1   不合法的协议族 
    // 0    转换失败
    // 1    转换成功
    int ret = inet_pton(AF_INET6, 
            addr.data(), 
            &m_sockaddr.sin6_addr.s6_addr);    

    if (ret == -1)
    {
        LOG_WARN_SYS << "address set failure, "
                     << "error desc is '" << strerror(errno) << "', "
                     << "error code is '" << errno << ". "
                     << "addr = " << addr << ".";
        m_addr_available = false;
        return;
    }

    if (ret == 0)
    {
        LOG_WARN_SYS << "address set failure. " 
                     << "addr = " << addr << ".";
        m_addr_available = false;
        return;
    }
    m_addr_available = true;
}

uint32_t SockAddrIn6::getPort() const
{
    uint32_t def = 0;

    if (!m_port_available)
    {
        LOG_WARN_SYS << "port invalid, "
                     << "return " << def << ".";    
        return def;
    }
    // 返回主机字节序
    return ntohs(m_sockaddr.sin6_port); 
}

void SockAddrIn6::setPort(uint32_t port)
{
    if (port < 0 || port > 65535)
    {
        LOG_WARN_SYS << "port set failure. "
                     << "port = " << port; 
        m_port_available = false;
        return;    
    }
    // 设置成网络字节序
    m_sockaddr.sin6_port = htons(port);    
    m_port_available = true;
}

void SockAddrIn6::setPort(const std::string& port)
{
    setPort(std::stoi(port));    
}

sockaddr* SockAddrIn6::get_c_data()
{
    if (!is_work())
        LOG_WARN_SYS << "SockAddrIn6 may not work.";    
    return (sockaddr*)&m_sockaddr;    
}

const sockaddr* SockAddrIn6::get_c_data() const
{
    if (!is_work())
        LOG_WARN_SYS << "SockAddrIn6 may not work.";    
    return (const sockaddr*)&m_sockaddr;    
}

void SockAddrIn6::set_c_data(const sockaddr* sockaddr)
{
    if (sockaddr->sa_family != AF_INET6)
    {
        LOG_WARN_SYS << "c_data set failure, "
                     << "family must be AF_INET6.";
        enable_work(false);
        return;
    } 
    
    m_sockaddr = *(sockaddr_in6*)sockaddr;
    enable_work(true);
}

socklen_t SockAddrIn6::length() const
{
    if (!is_work())
        LOG_WARN_SYS << "SockAddrIn6 may not work.";    
    return sizeof(m_sockaddr);    
}

std::string SockAddrIn6::format() const
{
    std::stringstream stream;
    stream << "[IPv6]"
           << "[" << getAddr() << "]"
           << ":" << getPort()
           << "(" << (is_work() ? "work" : "not work") << ")"
           << "|" << getDesc();
    return stream.str(); 
}

// ----------------------------------------------------------------------------

std::shared_ptr<SockAddrUn> SockAddrUn::LocalHost(const std::string& path)
{
    return std::make_shared<SockAddrUn>(path);    
}

SockAddrUn::SockAddrUn()
{
    enable_work(false);
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));    
    m_sockaddr.sun_family = AF_UNIX; 
}

SockAddrUn::SockAddrUn(const sockaddr* value)
{
    enable_work(true);
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));    
    set_c_data(value);
}

SockAddrUn::SockAddrUn(const sockaddr_in* value)
    : SockAddrUn((const sockaddr*)value) 
{    
}

SockAddrUn::SockAddrUn(const sockaddr_storage* value)
    : SockAddrUn((const sockaddr*)value)
{
}

SockAddrUn::SockAddrUn(const std::string& path)
{
    enable_work(false);
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));    
    m_sockaddr.sun_family = AF_UNIX; 
    setPath(path);
}

std::string SockAddrUn::getPath() const
{
    std::string def = "unknown";
    if (!m_path_available)
    {
        LOG_WARN_SYS << "path invalid, "
                     << "return '" << def << "'.";     
        return def;
    }
    return m_sockaddr.sun_path;
}

void SockAddrUn::setPath(const std::string& path)
{
    // struct sockaddr_un {
    //     sa_family_t sun_family;
    //     char        sun_path[108];
    // };
    // 路径的最大长度为 108 - 1，即 107 个字节

    if (path.size() >= sizeof(m_sockaddr.sun_path))
    {
        LOG_WARN_SYS << "path set failure. "
                     << "path = " << path;    
        m_path_available = false;
        return;
    }
    memcpy(m_sockaddr.sun_path, path.data(), path.size());
    m_path_available = true;
}

sockaddr* SockAddrUn::get_c_data()
{
    if (!is_work())
        LOG_WARN_SYS << "SockAddrUn may not work.";    
    return (sockaddr*)&m_sockaddr;    
}

const sockaddr* SockAddrUn::get_c_data() const
{
    if (!is_work())
        LOG_WARN_SYS << "SockAddrUn may not work.";    
    return (const sockaddr*)&m_sockaddr;    
}

void SockAddrUn::set_c_data(const sockaddr* sockaddr)
{
    if (sockaddr->sa_family != AF_UNIX)
    {
        LOG_WARN_SYS << "c_data set failure, "
                     << "family must be AF_UNIX.";
        enable_work(false); 
        return;
    } 
    
    m_sockaddr = *(sockaddr_un*)sockaddr;
    enable_work(true);
}

socklen_t SockAddrUn::length() const
{
    if (!is_work())
        LOG_WARN_SYS << "SockAddrUn may not work.";    
    return sizeof(m_sockaddr);    
}

std::string SockAddrUn::format() const
{
    std::stringstream stream;
    stream << "[UNIX]"
           << "[" << getPath() << "]"
           << "(" << (is_work() ? "work" : "not work") << ")"
           << "|" << getDesc();
    return stream.str();
}

// ----------------------------------------------------------------------------

}   // namespace net    
}   // namespace o7si
