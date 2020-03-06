/******************************************************************************
 * File: sockaddr.h
 * Description: sockaddr_xx 的面向对象封装。 
 * Author: o7si
 *****************************************************************************/
#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <iostream>
#include <string>

#include <log.h>

namespace o7si
{
namespace net
{

// sockaddr SockAddr
// sockaddr_in  SockAddrIn
// sockaddr_in6 SockAddrIn6
// sockaddr_un  SockAddrUn

class SockAddr
{
public:
    ~SockAddr() = default;
};

// IPv4
class SockAddrIn : public SockAddr
{
public:
    // flags = 0 时，表示 addr 为点分十进制的字符串
    // flags = 1 时，表示 addr 为域名的字符串
    // 默认值：flags = 0
    SockAddrIn(const std::string& addr, uint16_t port, int flags = 0);

    // 获取 IPv4 地址的点分十进制形式
    std::string getAddr() const;

    // 通过整型数字设置地址
    void setAddr(uint32_t addr);

    // 通过点分十进制的字符串设置地址
    void setAddrDot(const std::string& addr);

    // 通过域名的字符串设置地址
    void setAddrDomain(const std::string& addr);

    // 获取端口号
    uint16_t getPort() const;

    // 通过整型数字设置端口号
    void setPort(uint16_t port);

    // 通过字符串设置端口号
    void setPort(const std::string& port);

private:
    sockaddr_in m_sockaddr;
};

class SockAddrIn6 : public SockAddr
{
public:

private:
    sockaddr_in m_sockaddr;
};

class SockAddrUn : public SockAddr
{
public:

private:
    sockaddr_un m_sockaddr;
};

}   // namespace net
}   // namespace o7si
