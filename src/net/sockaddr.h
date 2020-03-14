/****************************************************************************** * File: sockaddr.h
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

#include <string>
#include <vector>

#include "../log.h"


/******************************************************************************
 * 类结构：
 *                         [SockAddr]                                        
 *                             /\
 *                           /    \
 *                         /        \
 *                       /            \
 *                 [SockAddrUn]  [SockAddrInet]
 *                                     /\
 *                                   /    \
 *                                 /        \
 *                               /            \
 *                         [SockAddrIn]  [SockAddrIn6]
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

    // 获取结果中对应的 IPv4 地址
    std::vector<HostInfo> get4() const
    {
        return m_ipv4;    
    }

    // 获取结果中对应的 IPv6 地址
    std::vector<HostInfo> get6() const
    {
        return m_ipv6;
    }

    // 结果中是否存在 IPv4 地址 
    bool has4() const
    {
        return !m_ipv4.empty();    
    }

    // 结果中是否存在 IPv6 地址 
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
    SockAddr() = default;

    ~SockAddr() = default;

    std::string getDesc() const
    {
        return m_desc;    
    }

    void setDesc(std::string desc)
    {
        m_desc = std::move(desc);    
    }

    // 获取内部的 sockaddr_xx 对象
    virtual sockaddr* get_c_data() = 0;

    virtual const sockaddr* get_c_data() const = 0;

    // 设置内部的 sockaddr_xx 对象
    // 该方法只会检查成员 family 的值是否正确
    // 如果正确，类会被强制变成可工作的
    // 如果不正确，类会被强制变成不可工作的
    // 其它参数不进行检查，需要调用方保证参数的合法性
    virtual void set_c_data(const sockaddr* sockaddr) = 0;

    void set_c_data(const sockaddr_storage* storage)
    {
        set_c_data((const sockaddr*)storage);    
    }

    // 获取内部 sockaddr_xx 对象的长度
    virtual socklen_t length() const = 0;

    // 强制修改类的可工作性
    virtual void enable_work(bool flag) = 0; 

    // 能够正常工作，即各项参数均未出错
    virtual bool is_work() const = 0;

private:    
    // 关于地址的描述信息
    std::string m_desc;
};

// ----------------------------------------------------------------------------

class SockAddrInet : public SockAddr
{
public:
    SockAddrInet() = default;

    ~SockAddrInet() = default;

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
}; 

// ----------------------------------------------------------------------------

// IPv4
class SockAddrIn : public SockAddrInet
{
public:
    static std::shared_ptr<SockAddrIn> LocalHost(uint32_t port);

    static std::shared_ptr<SockAddrIn> LocalHost(const std::string& port);

    SockAddrIn();

    // 以下 3 个构造函数均不检验参数的合法性
    // 实际调用的是 set_c_data 函数
    SockAddrIn(const sockaddr* value);
    SockAddrIn(const sockaddr_in* value);
    SockAddrIn(const sockaddr_storage* value);

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

    // 获取内部的 sockaddr_xx 对象
    sockaddr* get_c_data() override;

    const sockaddr* get_c_data() const override;

    // 设置内部的 sockaddr_xx 对象
    void set_c_data(const sockaddr* sockaddr) override;

    // 获取内部 sockaddr_xx 对象的长度
    socklen_t length() const override;

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

    void enable_work(bool flag) override
    {
        m_addr_available = flag;
        m_port_available = flag;    
    }

    bool is_work() const override
    {
        return m_addr_available &&
               m_port_available;    
    }

private:
    sockaddr_in m_sockaddr;
    bool m_addr_available;
    bool m_port_available;
};

// ----------------------------------------------------------------------------

class SockAddrIn6 : public SockAddrInet
{
public:
    static std::shared_ptr<SockAddrIn6> LocalHost(uint32_t port);

    static std::shared_ptr<SockAddrIn6> LocalHost(const std::string& port);

    SockAddrIn6();

    // 以下 3 个构造函数均不检验参数的合法性
    // 实际调用的是 set_c_data 函数
    SockAddrIn6(const sockaddr* value);
    SockAddrIn6(const sockaddr_in* value);
    SockAddrIn6(const sockaddr_storage* value);

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

    // 获取内部的 sockaddr_xx 对象
    sockaddr* get_c_data() override;

    const sockaddr* get_c_data() const override;

    // 设置内部的 sockaddr_xx 对象
    void set_c_data(const sockaddr* sockaddr) override;

    // 获取内部 sockaddr_xx 对象的长度
    socklen_t length() const override;

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

    void enable_work(bool flag) override
    {
        m_addr_available = flag;
        m_port_available = flag;    
    }

    bool is_work() const override
    {
        return m_addr_available &&
               m_port_available;    
    }

private:
    sockaddr_in6 m_sockaddr;
    bool m_addr_available;
    bool m_port_available;
};

// ----------------------------------------------------------------------------

// Local
class SockAddrUn : public SockAddr
{
public:
    // 静态方法名设置为 LocalHost 是为了保持代码的一致性
    static std::shared_ptr<SockAddrUn> LocalHost(const std::string& path);

    SockAddrUn();

    // 以下 3 个构造函数均不检验参数的合法性
    // 实际调用的是 set_c_data 函数
    SockAddrUn(const sockaddr* value);
    SockAddrUn(const sockaddr_in* value);
    SockAddrUn(const sockaddr_storage* value);

    SockAddrUn(const std::string& path);

    // 获取路径
    std::string getPath() const;

    // 设置路径
    void setPath(const std::string& path);

    // 获取内部的 sockaddr_xx 对象
    sockaddr* get_c_data() override;

    const sockaddr* get_c_data() const override;

    // 设置内部的 sockaddr_xx 对象
    void set_c_data(const sockaddr* sockaddr) override;

    // 获取内部 sockaddr_xx 对象的长度
    socklen_t length() const override;

    // 判断路径是否可用，即设置值的过程中未出现错误
    bool path_is_available() const
    {
        return m_path_available;    
    }

    void enable_work(bool flag) override
    {
        m_path_available = flag; 
    }

    bool is_work() const override
    {
        return m_path_available;
    }

private:
    sockaddr_un m_sockaddr;
    bool m_path_available;
};

// ----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& stream, const SockAddrIn& sockaddr);
std::ostream& operator<<(std::ostream& stream, const SockAddrIn6& sockaddr);
std::ostream& operator<<(std::ostream& stream, const SockAddrUn& sockaddr);

// ----------------------------------------------------------------------------

}   // namespace net
}   // namespace o7si
