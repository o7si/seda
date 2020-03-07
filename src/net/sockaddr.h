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
#include <vector>

#include <log.h>


/******************************************************************************
 *
 *           [SockAddr]
 *               /\
 *             /    \
 *           /        \
 *         /            \
 *   [SockAddrUn]  [SockAddrInet]
 *                       /\
 *                     /    \
 *                   /        \
 *                 /            \
 *           [SockAddrIn]  [SockAddrIn6]
 *
 *****************************************************************************/

namespace o7si
{
namespace net
{


// ----------------------------------------------------------------------------

// 查找主机对应的 IPv4 以及 IPv6 地址
class Lookup
{
public:
    class HostInfo
    {
    public:
        HostInfo(std::string name, std::string addr);

        std::string getName() const
        {
            return m_name;    
        }

        void setName(std::string name)
        {
            m_name = std::move(name); 
        }

        std::string getAddr() const
        {
            return m_addr;    
        }

        void setAddr(std::string addr)
        {
            m_addr = std::move(addr);    
        }

    private:
        // 主机名称
        std::string m_name;
        // 主机地址
        std::string m_addr;
    };

    // 可以传入主机名，例如 localhost
    // 可以传入 IPv4 地址，例如 127.0.0.1
    // 可以传入 IPv6 地址，例如 ::1
    void lookup(const std::string& host); 

    std::vector<HostInfo> get4() const
    {
        return m_ipv4;    
    }

    std::vector<HostInfo> get6() const
    {
        return m_ipv6;
    }

    bool has4() const
    {
        return !m_ipv4.empty();    
    }

    bool has6() const
    {
        return !m_ipv6.empty();    
    }

private:
    // 查找到的 IPv4 地址
    std::vector<HostInfo> m_ipv4;
    // 查找到的 IPv6 地址
    std::vector<HostInfo> m_ipv6;
};

// ----------------------------------------------------------------------------

class SockAddr
{
public:
    ~SockAddr() = default;

    std::string getDesc() const
    {
        return m_desc;    
    }

    void setDesc(std::string desc)
    {
        m_desc = std::move(desc);    
    }

private:    
    // 关于地址的描述信息
    std::string m_desc;
};

class SockAddrInet : public SockAddr
{
public:
    SockAddrInet();

    ~SockAddrInet() = default;

    // 判断地址是否可用，即设置值的过程中未出现错误
    bool addr_is_available() const
    {
        return m_addr_available; 
    }

    // 判断端口是否可用，即设置值的过程中未出现错误
    bool port_is_available() const
    {
        return m_port_available;    
    }

    // 获取地址的字符串表示形式
    virtual std::string getAddr() const = 0;

    // 通过字符串设置地址
    virtual void setAddr(const std::string& addr) = 0;

    // 获取端口号
    virtual uint32_t getPort() const = 0;

    // 通过整型数字设置端口号
    virtual void setPort(uint32_t port) = 0;

    // 通过字符串设置端口号
    virtual void setPort(const std::string& port) = 0;

protected:
    bool m_addr_available;
    bool m_port_available;
}; 

// IPv4
class SockAddrIn : public SockAddrInet
{
public:
    // 参数 addr 为点分十进制的 IPv4 地址
    // 参数 port 为端口号
    SockAddrIn(const std::string& addr, uint32_t port);

    SockAddrIn(const std::string& addr, const std::string& port);

    // 获取 IPv4 地址的点分十进制形式
    std::string getAddr() const override;

    // 通过点分十进制的字符串设置 IPv4 地址
    void setAddr(const std::string& addr) override;

    // 获取端口号
    uint32_t getPort() const override;

    // 通过整型数字设置端口号
    void setPort(uint32_t port) override;

    // 通过字符串设置端口号
    void setPort(const std::string& port) override;

private:
    sockaddr_in m_sockaddr;
};

class SockAddrIn6 : public SockAddrInet
{
public:
    // 参数 addr 为冒号十六进制的 IPv6 地址
    // 参数 port 为端口号
    SockAddrIn6(const std::string& addr, uint32_t port);

    SockAddrIn6(const std::string& addr, const std::string& port);

    // 获取 IPv6 地址的冒号十六进制形式
    std::string getAddr() const override;

    // 通过冒号十六进制的字符串设置 IPv6 地址
    void setAddr(const std::string& addr) override;

    // 获取端口号
    uint32_t getPort() const override;

    // 通过整型数字设置端口号
    void setPort(uint32_t port) override;

    // 通过字符串设置端口号
    void setPort(const std::string& port) override;

private:
    sockaddr_in6 m_sockaddr;
};

class SockAddrUn : public SockAddr
{
public:

private:
    sockaddr_un m_sockaddr;
};

}   // namespace net
}   // namespace o7si
