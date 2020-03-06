#include "sockaddr.h"


namespace o7si
{
namespace net
{
    
// ----------------------------------------------------------------------------

// SockAddr

// ----------------------------------------------------------------------------

SockAddrIn::SockAddrIn(const std::string& addr, uint16_t port, int flags)
{
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));    
    m_sockaddr.sin_family = AF_INET;
    setPort(port);

    // 当 flags 的值不合法时使用默认值
    if (flags != 0 && flags != 1)
        flags = 0;

    if (flags == 0)
        setAddrDot(addr);
    else if (flags == 1)
        setAddrDomain(addr);
}

std::string SockAddrIn::getAddr() const
{
    return inet_ntoa(m_sockaddr.sin_addr);
}

void SockAddrIn::setAddr(uint32_t addr)
{
    // 设置成网络序
    m_sockaddr.sin_addr.s_addr = htonl(addr);    
}

void SockAddrIn::setAddrDot(const std::string& addr)
{
    // -1   不合法的协议族 
    // 0    转换失败
    // 1    转换成功
    int ret = inet_pton(AF_INET, 
            addr.data(), 
            &m_sockaddr.sin_addr.s_addr);    
    LOG_WARN_CHECK_TRUE_SYS(ret == -1)
        << strerror(errno) << "(" << errno << ")";
    LOG_WARN_CHECK_TRUE_SYS(ret == 0)
        << "address set failure: " << addr;
    LOG_INFO_CHECK_TRUE_SYS(ret == 1)
        << "address set success: " << addr;
}

void SockAddrIn::setAddrDomain(const std::string& addr)
{
    // struct hostent
    // {
    //     char *h_name;
    //     char **h_aliases;
    //     int h_addrtype;	
    //     int h_length;
    //     char **h_addr_list;	
    // };

    // null     转换失败
    // non-null 转换成功
    hostent* host = gethostbyname(addr.data());

    if (host == nullptr)
    {
        LOG_WARN_SYS << "address set failure: " << addr; 
        return;
    }

    char buffer[INET_ADDRSTRLEN];
    memset(buffer, 0, sizeof(buffer));

    LOG_DEBUG_SYS << "host_info";
    LOG_DEBUG_SYS << "host_name: " << host->h_name;
    size_t alias_count = 0;
    for (char** ptr = host->h_aliases; *ptr != nullptr; ++ ptr)
        LOG_DEBUG_SYS << "host_alise(" << ++ alias_count << "): " << *ptr;
    size_t addr_count = 0;
    for (char** ptr = host->h_addr_list; *ptr != nullptr; ++ ptr)
        LOG_DEBUG_SYS << "host_addr(" << ++ addr_count << "): "
            << inet_ntop(host->h_addrtype, *ptr,
                    buffer, sizeof(buffer));

    setAddrDot(inet_ntop(host->h_addrtype, host->h_addr_list[0], 
                buffer, sizeof(buffer)));
}

uint16_t SockAddrIn::getPort() const
{
    // 返回主机序
    return ntohs(m_sockaddr.sin_port);    
}

void SockAddrIn::setPort(uint16_t port)
{
    // 设置成网络序
    m_sockaddr.sin_port = htons(port);    
}

void SockAddrIn::setPort(const std::string& port)
{
    setPort(std::stoi(port));    
}


// ----------------------------------------------------------------------------

// SockAddrIn6

// ----------------------------------------------------------------------------

// SockAddrUn

// ----------------------------------------------------------------------------

}   // namespace net    
}   // namespace o7si
