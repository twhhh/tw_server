#pragma once
#include <memory>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include "address.h"
#include "Noncopyable.h"
namespace tw{
class Socket : public std::enable_shared_from_this<Socket>, Noncopyable{
public:
    using ptr = std::shared_ptr<Socket>;
    using weak_ptr = std::weak_ptr<Socket>;
    enum Type{
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM
    };
    enum Family{
        IPv4 = AF_INET,
        IPv6 = AF_INET6,
        UNIX = AF_UNIX
    };
    static Socket::ptr CreateTCP(tw::Address::ptr address);
    static Socket::ptr CreateUDP(tw::Address::ptr address);
    static Socket::ptr CreateTCPSocket();
    static Socket::ptr CreateUDPSocket();
    static Socket::ptr CreateTCPSocket6();
    static Socket::ptr CreateUDPSocket6();
    static Socket::ptr CreateUnixTCPSocket();
    static Socket::ptr CreateUnixUDPSocket();
    Socket(int family, int type, int protocol = 0);
    ~Socket();
    int64_t getSendTimeout();
    void setSendTimeout(int64_t v);
    int64_t getRecvTimeout();
    void setRecvTimeout(int64_t v);
    bool getOption(int level, int optname, void* optval, socklen_t* optlen);
    template<class T>
    bool getOption(int level, int optname, void* optval){
        socklen_t length = sizeof(optval);
        return getOption(level, optname, optval, &length);
    } 
    bool setOption(int level, int optname,
                      const void *optval, socklen_t optlen);
    template<class T>
    bool setOption(int level, int optname, const T& optval){
        return setOption(level, optname, &optval, sizeof(T));
    }

    Socket::ptr accept();
    bool bind(const Address::ptr& addr);
    bool connect(const Address::ptr& addr, uint64_t timeout_ms = -1);
    bool listen(int backlog = SOMAXCONN);
    bool close();

    int send(const void* buffer, size_t length, int flags = 0);
    int send(const iovec* buffers, size_t length, int flags = 0);
    int sendTo(const void* buffer, size_t length, const Address::ptr to, int flags = 0);
    int sendTo(const iovec* buffers, size_t length, const Address::ptr to, int flags = 0);
    int recv(void *buffer, size_t length, int flags = 0);
    int recv(iovec* buffers, size_t length, int flags = 0);
    int recvFrom(void *buffer, size_t length, const Address::ptr from, int flags = 0);
    int recvFrom(iovec *buffers, size_t length, const Address::ptr from, int flags = 0);
    Address::ptr getLocalAddress();
    Address::ptr getremoteAddress();
    int getSocket() const {return m_sock;}
    int getFamily() const {return m_family;}
    int getType() const {return m_type;}
    int getProtocol() const {return m_protocol;}
    bool isConnected() const {return m_isConnected;}
    bool isValid() const {return m_sock != -1;}
    int getError();
    std::ostream& dump(std::ostream& os) const;

    bool cancelRead();
    bool cancelWrite();
    bool cancelAccept();
    bool cancelAll();
private:
    void initSock();
    void newSock();
    bool init(int sock);
private:
    int m_sock;
    int m_family;
    int m_type;
    int m_protocol;
    bool m_isConnected;
    Address::ptr m_localAddress;
    Address::ptr m_remoteAddress;
};

std::ostream& operator<<(std::ostream& os, const Socket& sock);
}