#include "socket.h"
#include "log.h"
#include "hook.h"
#include "fd_manager.h"
#include "iomanager.h"

#include <limits.h>
namespace tw{
static Logger::ptr m_logger = TW_LOG_NAME("system");
Socket::ptr Socket::CreateTCP(tw::Address::ptr address){
    Socket::ptr sock(new Socket(address->getFamily()
        , TCP, 0));
    return sock;
}
Socket::ptr Socket::CreateUDP(tw::Address::ptr address){
    Socket::ptr sock(new Socket(address->getFamily()
        , UDP, 0));
    return sock;
}
Socket::ptr Socket::CreateTCPSocket(){
    Socket::ptr sock(new Socket(IPv4, TCP, 0));
    return sock;
}
Socket::ptr Socket::CreateUDPSocket(){
    Socket::ptr sock(new Socket(IPv4, UDP, 0));
    return sock;
}
Socket::ptr Socket::CreateTCPSocket6(){
    Socket::ptr sock(new Socket(IPv6, TCP, 0));
    return sock;
}
Socket::ptr Socket::CreateUDPSocket6(){
    Socket::ptr sock(new Socket(IPv6, UDP, 0));
    return sock;
}
Socket::ptr Socket::CreateUnixTCPSocket(){
    Socket::ptr sock(new Socket(UNIX, TCP, 0));
    return sock;
}
Socket::ptr Socket::CreateUnixUDPSocket(){
    Socket::ptr sock(new Socket(UNIX, UDP, 0));
    return sock;
}
Socket::Socket(int family, int type, int protocol)
    :m_sock(-1)
    ,m_family(family)
    ,m_type(type)
    ,m_protocol(protocol)
    ,m_isConnected(false){
}
Socket::~Socket(){
    close();
}
int64_t Socket::getSendTimeout(){
    FdCtx::ptr ctx = FdMgr::GetInstance()->get(m_sock);
    if(ctx){
        return ctx->getTimeout(SO_SNDTIMEO);
    }
    return -1;
}
void Socket::setSendTimeout(int64_t v){
    struct timeval tv{int(v / 1000), int(v % 1000 * 1000)};
    setOption(SOL_SOCKET, SO_SNDTIMEO, tv);
}
int64_t Socket::getRecvTimeout(){
    FdCtx::ptr ctx = FdMgr::GetInstance()->get(m_sock);
    if(ctx){
        return ctx->getTimeout(SO_RCVTIMEO);
    }
    return -1;
}
void Socket::setRecvTimeout(int64_t v){
    struct timeval tv{int(v / 1000), int(v % 1000 * 1000)};
    setOption(SOL_SOCKET, SO_RCVTIMEO, tv);
}
bool Socket::getOption(int level, int optname, void* optval, socklen_t* optlen){
    int rt = getsockopt(m_sock, level, optname, optval, (socklen_t*)optlen);
    if(rt) {
        TW_LOG_DEBUG(m_logger) << "getOption sock = " << m_sock
            << "level = " << level << "optname = " << optname
            << "errno = " << errno << "errstr = " << strerror(errno);
        return false;
    }
    return true;
}
bool Socket::setOption(int level, int optname,
                      const void *optval, socklen_t optlen){
    int rt = setsockopt(m_sock, level, optname, optval, (socklen_t)optlen);
    if(rt) {
        TW_LOG_DEBUG(m_logger) << "setOption sock = " << m_sock
            << "level = " << level << "optname = " << optname
            << "errno = " << errno << "errstr = " << strerror(errno);
        return false;
    }
    return true;
}

Socket::ptr Socket::accept(){
    Socket::ptr sock(new Socket(m_family, m_type, m_protocol));
    int newsock = ::accept(m_sock, nullptr, nullptr);
    if(newsock == -1){
        TW_LOG_ERROR(m_logger) << "accept(" << m_sock << ") errno"
            << errno << "errstr = " << strerror(errno);
        return nullptr;
    }
    if(sock->init(newsock)){
        return sock;
    }
    return nullptr;
}
bool Socket::bind(const Address::ptr& addr){
    if(!isValid()){
        newSock();
        if((!isValid())){
            return false;
        }
    }

    if(addr->getFamily() != m_family){
        TW_LOG_ERROR(m_logger) << "bind sock.family("
            << m_family <<") addr.family(" << addr->getFamily()
            << ") not equal, addr = " << addr->toString();
        return false;
    }
    if(::bind(m_sock, addr->getAddr(), addr->getAddrLen())){
        TW_LOG_ERROR(m_logger) << "bind error errno = "
        << errno << " errstr " << strerror(errno);
        return false;
    }
    getLocalAddress();
    return true;
}
bool Socket::connect(const Address::ptr& addr, uint64_t timeout_ms){
    if(!isValid()){
        newSock();
        if(!isValid()){
            return false;
        }
    }
    if(addr->getFamily() != m_family){
        TW_LOG_ERROR(m_logger) << "bind sock.family("
            << m_family <<") addr.family(" << addr->getFamily()
            << ") not equal, addr = " << addr->toString();
        return false;
    }
    if(timeout_ms == (uint64_t)-1){
        if(::connect(m_sock, addr->getAddr(), addr->getAddrLen())){
            TW_LOG_ERROR(m_logger) << "sock = " << m_sock
                << "connect (" << addr->toString()
                << ") errno = " << errno << "errstr = " << strerror(errno);
            close();
            return false; 
        }
    } else {
        if(::connect_with_timeout(m_sock, addr->getAddr()
            , addr->getAddrLen(), timeout_ms)){
            TW_LOG_ERROR(m_logger) << "sock = " << m_sock
            << "connect (" << addr->toString()
            << ") timeout = " << timeout_ms << "errno = " << errno 
            << "errstr = " << strerror(errno);
            close();
            return false;
        }
    }
    m_isConnected = true;
    getLocalAddress();
    getremoteAddress();
    return true;
}
bool Socket::listen(int backlog){
    if(!isValid()){
        TW_LOG_ERROR(m_logger) << "listen error sockfd = -1";
        return false;
    }
    if(::listen(m_sock, backlog)){
        TW_LOG_ERROR(m_logger) << "listen error errno = " << errno
            << "errstr = " << strerror(errno);
        return false;
    }
    return true;
}
bool Socket::close(){
    if(!m_isConnected && m_sock == -1){
        return true;
    }
    m_isConnected = false;
    if(m_sock != -1){
        ::close(m_sock);
        m_sock = -1;
    }
    return false;
}
int Socket::send(const void* buffer, size_t length, int flags){
    if(isConnected()){
        return ::send(m_sock, buffer, length, flags);
    }
    return -1;
}
int Socket::send(const iovec* buffers, size_t length, int flags){
    if(isConnected()){
        msghdr msg;
        memset(&msg, 0, sizeof(msghdr));
        msg.msg_iov = (iovec*)buffers;
        msg.msg_iovlen = length;
        return ::sendmsg(m_sock, &msg, flags);
    }
    return -1;
}
int Socket::sendTo(const void* buffer, size_t length, const Address::ptr to, int flags){
    if(isConnected()){
        return ::sendto(m_sock, buffer, length, flags
            ,to->getAddr(), to->getAddrLen());
    }
    return -1;
}
int Socket::sendTo(const iovec* buffers, size_t length, const Address::ptr to, int flags){
    if(isConnected()){
        msghdr msg;
        memset(&msg, 0, sizeof(msghdr));
        msg.msg_iov = (iovec*)buffers;
        msg.msg_iovlen = length;
        msg.msg_name = to->getAddr();
        msg.msg_namelen = to->getAddrLen();
        return ::sendmsg(m_sock, &msg, flags);
    }
    return -1;
}
int Socket::recv(void *buffer, size_t length, int flags){
    if(isConnected()){
        return ::recv(m_sock, buffer, length, flags);
    }
    return -1;
}
int Socket::recv(iovec* buffers, size_t length, int flags){
    if(isConnected()){
        msghdr msg;
        memset(&msg, 0, sizeof(msghdr));
        msg.msg_iov = (iovec*)buffers;
        msg.msg_iovlen = length;
        return ::recvmsg(m_sock, &msg, flags);
    }
    return -1;
}
int Socket::recvFrom(void *buffer, size_t length, const Address::ptr from, int flags){
    if(isConnected()){
        socklen_t len = from->getAddrLen();
        return ::recvfrom(m_sock, buffer, length, flags
            , from->getAddr(), &len);
    }
    return -1;
}
int Socket::recvFrom(iovec *buffers, size_t length, const Address::ptr from, int flags){
    if(isConnected()){
        msghdr msg;
        memset(&msg, 0, sizeof(msghdr));
        msg.msg_iov = (iovec*)buffers;
        msg.msg_iovlen = length;
        msg.msg_name = from->getAddr();
        msg.msg_namelen = from->getAddrLen();
        return ::recvmsg(m_sock, &msg, flags);
    }
    return -1;
}
Address::ptr Socket::getLocalAddress(){
    if(m_localAddress){
        return m_localAddress;
    }
    Address::ptr result;
    switch(m_family){
        case AF_INET:
        result.reset(new IPv4Address());
        break;
        case AF_INET6:
        result.reset(new IPv6Address());
        break;
        case AF_UNIX:
        result.reset(new UnixAddress());
        default:
        result.reset(new UnknownAddress(m_family));
        break;
    }
    socklen_t addrlen = result->getAddrLen();
    if(getsockname(m_sock, result->getAddr(), &addrlen)){
        TW_LOG_ERROR(m_logger) << "getsockname error sock = "
            << m_sock << " errno = " << "errstr = " << strerror(errno);
        return Address::ptr(new UnknownAddress(m_family)); 
    }
    if(m_family == AF_UNIX){
        UnixAddress::ptr addr = std::dynamic_pointer_cast<UnixAddress>(result);
        addr->setAddrLen(addrlen);
    }
    m_localAddress = result;
    return m_localAddress;
}
Address::ptr Socket::getremoteAddress(){
    if(m_remoteAddress){
        return m_remoteAddress;
    }
    Address::ptr result;
    switch(m_family){
        case AF_INET:
        result.reset(new IPv4Address());
        break;
        case AF_INET6:
        result.reset(new IPv6Address());
        break;
        case AF_UNIX:
        result.reset(new UnixAddress());
        default:
        result.reset(new UnknownAddress(m_family));
        break;
    }
    socklen_t addrlen = result->getAddrLen();
    if(getpeername(m_sock, result->getAddr(), &addrlen)){
        TW_LOG_ERROR(m_logger) << "getpeername error sock = "
            << m_sock << " errno = " << "errstr = " << strerror(errno);
        return Address::ptr(new UnknownAddress(m_family)); 
    }
    if(m_family == AF_UNIX){
        UnixAddress::ptr addr = std::dynamic_pointer_cast<UnixAddress>(result);
        addr->setAddrLen(addrlen);
    }
    m_remoteAddress = result;
    return m_remoteAddress;
}
int Socket::getError(){
    int error = 0;
    socklen_t len = sizeof(error);
    if(!getOption(SOL_SOCKET, SO_ERROR, &error, &len)){
        error = errno;
    }
    return error;
}

std::ostream& Socket::dump(std::ostream& os) const {
    os << "[Socket sock = " << m_sock 
        << "is connected= " << m_isConnected
        << "family = " << m_family
        << "type = " << m_type
        << "protocol = " << m_protocol;
    if(m_localAddress){
        os << "localAddress = " << m_localAddress->toString();
    }
    if(m_remoteAddress){
        os << "remoteAddress = " << m_remoteAddress->toString();
    }
    return os;
}

bool Socket::cancelRead(){
    return IOManager::GetThis()->cancelEvent(m_sock, tw::IOManager::READ);
}
bool Socket::cancelWrite(){
    return IOManager::GetThis()->cancelEvent(m_sock, IOManager::WRITE);
}
bool Socket::cancelAccept(){
    return cancelRead();
}
bool Socket::cancelAll(){
    return IOManager::GetThis()->cancelAll(m_sock);
}
void Socket::initSock(){
    int val = 1;
    setOption(SOL_SOCKET, SO_REUSEADDR, val);
    if(m_type == SOCK_STREAM){
        setOption(IPPROTO_TCP, TCP_NODELAY, val);
    }
}
void Socket::newSock(){
    m_sock = socket(m_family, m_type, m_protocol);
    if(m_sock != -1){
        initSock();
    } else {
        TW_LOG_ERROR(m_logger) << "socket(" << m_family 
            << ", " << m_type << ", " << m_protocol 
            << "), errno = " << errno << strerror(errno);
    }
}
bool Socket::init(int sock){
    FdCtx::ptr ctx = FdMgr::GetInstance()->get(sock);
    if(ctx && ctx->isSocket() && !ctx->isClosed()){
        m_sock = sock;
        m_isConnected = true;
        initSock();
        getLocalAddress();
        getremoteAddress();
        return true;
    } 
    return false;
}

std::ostream& operator<<(std::ostream& os, const Socket& sock){
    return sock.dump(os);
}
}