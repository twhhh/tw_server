#include "hook.h"
#include "iomanager.h"
#include "log.h"
#include "macro.h"
#include "fiber.h"
#include "scheduler.h"
#include "fd_manager.h"
#include <dlfcn.h> 
static tw::Logger::ptr m_logger = TW_LOG_NAME("system");
namespace tw{
static thread_local bool t_hook_enable = false;
bool is_hook_enable(){
    return t_hook_enable;
}

void set_hook_enable(bool flag){
    t_hook_enable = flag;
}
#define HOOK_FUN(XX)\
    XX(sleep)\
    XX(usleep)\
    XX(socket)\
    XX(connect)\
    XX(accept)\
    XX(read)\
    XX(readv)\
    XX(recv)\
    XX(recvfrom)\
    XX(recvmsg)\
    XX(write)\
    XX(writev)\
    XX(send)\
    XX(sendto)\
    XX(sendmsg)\
    XX(close)\
    XX(fcntl)\
    XX(getsockopt)\
    XX(setsockopt)\


void hook_init(){
    static bool is_inited = false;
    if(is_inited){
        return;
    }
#define XX(name) name ## _f = (name ## _fun)dlsym(RTLD_NEXT, #name);
    HOOK_FUN(XX);
#undef XX
}
struct _HookIniter{
    _HookIniter(){
        hook_init();
    }
};

static _HookIniter s_hook_initer;
}

struct timer_info{
    int cancelled = 0;
};

template <typename OriginFun, typename... Args>
static ssize_t do_io(int fd, OriginFun fun, const char* hook_fun_name
    , uint32_t event, int timeout_so, Args&&... args){
    if(!tw::t_hook_enable){
        return fun(fd, std::forward<Args>(args)...);
    }
    tw::FdCtx::ptr ctx = tw::FdMgr::GetInstance()->get(fd);
    if(!ctx){
        return fun(fd, std::forward<Args>(args)...);
    }
    if(ctx->isClosed()){
        errno = EBADF;
        return -1;
    }
    if(!ctx->isSocket() || ctx->getUsrNonBlock()){
        return fun(fd, std::forward<Args>(args)...);
    }
    std::shared_ptr<timer_info> tinfo(new timer_info);
    uint64_t io = ctx->getTimeout(timeout_so);
retry:
    ssize_t n = fun(fd, std::forward<Args>(args)...);
    while(n == -1 && errno == EINTR){
        n = fun(fd, std::forward<Args>(args)...);
    }
    if(n == -1 && errno == EAGAIN){
        tw::IOManager* iom = tw::IOManager::GetThis();
        tw::Timer::ptr timer;
        std::weak_ptr<timer_info> winfo(tinfo);
        if(io != (uint64_t)-1){
            timer = iom->addConditionTimer(io
                , [winfo, fd, iom, event](){
                    auto t = winfo.lock();
                    if(!t || t->cancelled){
                        return;
                    }
                    t->cancelled = ETIMEDOUT;
                    iom->cancelEvent(fd, (tw::IOManager::Event)event);
                }, winfo);
        }
        int rt = iom->addEvent(fd, (tw::IOManager::Event)event);
        if(rt){
            TW_LOG_ERROR(m_logger) << hook_fun_name 
                << "addEvent(" << fd << "," << event << ")";
            if(timer){
                timer->cancel();
            } 
            return -1;
        }else{
            tw::Fiber::YieldToHold();
            tw::Fiber::GetThis()->setState(tw::Fiber::EXEC);
            if(timer){
                timer->cancel();
            }
            if(tinfo->cancelled){
                errno = tinfo->cancelled;
                return -1;
            }
            goto retry;
        }

    }
    return n;
}
extern "C"{
#define XX(name) name ## _fun name ## _f = nullptr;
    HOOK_FUN(XX);
#undef XX

unsigned int sleep(unsigned int seconds){
    if(!tw::is_hook_enable()){
        return sleep_f(seconds);
    }
    tw::Fiber::ptr fiber =  tw::Fiber::GetThis();
    tw::IOManager* iom = tw::IOManager::GetThis();
    TW_ASSERT(iom != nullptr);
    iom->addTimer(seconds * 1000, std::bind(
    (void(tw::Scheduler::*)(tw::Fiber::ptr))
    &tw::IOManager::schedule
    , iom, fiber));
    tw::Fiber::YieldToHold();
    return 0;
}

int usleep(useconds_t usec){
    if(!tw::is_hook_enable()){
        return sleep_f(usec);
    }
    tw::Fiber::ptr fiber =  tw::Fiber::GetThis();
    tw::IOManager* iom = tw::IOManager::GetThis();
    TW_ASSERT(iom != nullptr);
    iom->addTimer(usec / 1000, std::bind(
    (void(tw::Scheduler::*)(tw::Fiber::ptr))
    &tw::IOManager::schedule
    , iom, fiber));
    tw::Fiber::YieldToHold();
    return 0;
}

int socket(int domain, int type, int protocol){
    if(!tw::is_hook_enable()){
        return socket_f(domain, type, protocol);
    }
    int fd = socket_f(domain, type, protocol);
    if(fd == -1){
        return fd;
    }
    tw::FdMgr::GetInstance()->get(fd, true);
    return fd;
}

int connect_with_timeout(int sockfd, const struct sockaddr* addr, socklen_t addrlen, uint64_t timeout_ms){
    if(!tw::is_hook_enable()){
        return connect_f(sockfd, addr, addrlen);
    }
    tw::FdCtx::ptr ctx = tw::FdMgr::GetInstance()->get(sockfd);
    if(!ctx || ctx->isClosed()){
        errno = EBADF;
        return -1;
    }
    if(!ctx->isSocket()){
        return connect_f(sockfd, addr, addrlen);
    }
    if(ctx->getUsrNonBlock()){
        return connect_f(sockfd, addr, addrlen);
    }
    int n = connect_f(sockfd, addr, addrlen);
    if(n == 0){
        return 0;
    } else if(n != -1 || errno != EINPROGRESS){
        return n;
    }
    tw::IOManager* iom = tw::IOManager::GetThis();
    tw::Timer::ptr timer;
    std::shared_ptr<timer_info> tinfo(new timer_info);
    std::weak_ptr<timer_info> winfo(tinfo);
    if(timeout_ms != (uint64_t)-1){
        timer = iom->addConditionTimer(timeout_ms, [winfo, sockfd, iom](){
            auto t = winfo.lock();
            if(!t || t->cancelled){
                return;
            }
            t->cancelled = ETIMEDOUT;
            iom->cancelEvent(sockfd, tw::IOManager::WRITE);
        }, winfo);
    }
    int rt = iom->addEvent(sockfd, tw::IOManager::WRITE);
    if(rt){
        TW_LOG_ERROR(m_logger) << "connect addEvent(" 
            << sockfd << ", write";
        timer->cancel();
    }else{
        tw::Fiber::YieldToHold();
        if(timer){
            timer->cancel();
        }
        if(tinfo->cancelled){
            errno = tinfo->cancelled;
            return -1;
        }
    }
    int error = 0;
    socklen_t len = sizeof(int);
    if(-1 == getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len)){
        return -1;
    }
    if(!error){
        return 0;
    }else{
        errno = error;
        return -1;
    }
}
int connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen){
    return connect_with_timeout(sockfd, addr, addrlen, 2000);
}

int accept(int s, struct sockaddr *addr, socklen_t *addrlen){
    int fd = do_io(s, accept_f, "accept", tw::IOManager::READ
        , SO_RCVTIMEO, addr, addrlen);
    if(fd >= 0){
        tw::FdMgr::GetInstance()->get(fd, true);
    }
    return fd;
}

ssize_t read(int fd, void *buf, size_t count){
    return do_io(fd, read_f, "read", tw::IOManager::READ, SO_RCVTIMEO
        , buf, count);
}

ssize_t readv(int fd, const struct iovec *iov, int iovcnt) {
    return do_io(fd, readv_f, "readv", tw::IOManager::READ, SO_RCVTIMEO, iov, iovcnt);
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags) {
    return do_io(sockfd, recv_f, "recv", tw::IOManager::READ, SO_RCVTIMEO, buf, len, flags);
}

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) {
    return do_io(sockfd, recvfrom_f, "recvfrom", tw::IOManager::READ, SO_RCVTIMEO, buf, len, flags, src_addr, addrlen);
}

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags) {
    return do_io(sockfd, recvmsg_f, "recvmsg", tw::IOManager::READ, SO_RCVTIMEO, msg, flags);
}

ssize_t write(int fd, const void *buf, size_t count) {
    return do_io(fd, write_f, "write", tw::IOManager::WRITE, SO_SNDTIMEO, buf, count);
}

ssize_t writev(int fd, const struct iovec *iov, int iovcnt) {
    return do_io(fd, writev_f, "writev", tw::IOManager::WRITE, SO_SNDTIMEO, iov, iovcnt);
}

ssize_t send(int s, const void *msg, size_t len, int flags) {
    return do_io(s, send_f, "send", tw::IOManager::WRITE, SO_SNDTIMEO, msg, len, flags);
}

ssize_t sendto(int s, const void *msg, size_t len, int flags, const struct sockaddr *to, socklen_t tolen) {
    return do_io(s, sendto_f, "sendto", tw::IOManager::WRITE, SO_SNDTIMEO, msg, len, flags, to, tolen);
}

ssize_t sendmsg(int s, const struct msghdr *msg, int flags) {
    return do_io(s, sendmsg_f, "sendmsg", tw::IOManager::WRITE, SO_SNDTIMEO, msg, flags);
}

int close(int fd){
    if(!tw::is_hook_enable()){
        return close_f(fd);
    }
    tw::FdCtx::ptr ctx = tw::FdMgr::GetInstance()->get(fd);
    if(ctx){
        tw::IOManager* iom = tw::IOManager::GetThis();
        if(iom){
            iom->cancelAll(fd);
        }
        tw::FdMgr::GetInstance()->del(fd);
    }
    return close_f(fd);
}

int fcntl(int fd, int cmd, ... /* arg */ ){
    va_list va;
    va_start(va, cmd);
    switch(cmd){
        case F_SETFL:
            {
                int arg = va_arg(va, int);
                va_end(va);
                tw::FdCtx::ptr ctx = tw::FdMgr::GetInstance()->get(fd);
                if(!ctx || ctx->isClosed() || !ctx->isSocket()){
                    return fcntl_f(fd, cmd, arg);
                }
                TW_LOG_ERROR(m_logger) << "fcntl error";
                ctx->setUsrNonBlock(arg & O_NONBLOCK);
                if(ctx->getSysNonBlock()){
                    arg |= O_NONBLOCK;
                }else{
                    arg &= ~O_NONBLOCK;
                }
                return fcntl_f(fd, cmd, arg);
            }
            break;
        case F_GETFL:
            {
                va_end(va);
                int arg = fcntl_f(fd, cmd);
                tw::FdCtx::ptr ctx = tw::FdMgr::GetInstance()->get(fd);
                if(!ctx || ctx->isClosed() || !ctx->isSocket()){
                    return arg;
                }
                if(ctx->getUsrNonBlock()){
                    return arg | O_NONBLOCK;
                }else{
                    return arg & ~O_NONBLOCK;
                }
            }
            break;
        case F_DUPFD:
        case F_DUPFD_CLOEXEC:
        case F_SETFD:
        case F_SETOWN:
        case F_SETSIG:
        case F_SETLEASE:
        case F_NOTIFY:
#ifdef F_SETPIPE_SZ
        case F_SETPIPE_SZ:
#endif
        {
            int arg = va_arg(va, int);
            va_end(va);
            fcntl_f(fd, cmd, arg);
        }
        break;
        case F_GETFD:
        case F_GETOWN:
        case F_GETSIG:
        case F_GETLEASE:
#ifdef F_GETPIPE_SZ
        case F_GETPIPE_SZ:
#endif
        {
            va_end(va);
            fcntl_f(fd, cmd);
        }
        break;
        case F_SETLK:
        case F_SETLKW:
        case F_GETLK:
        {
            struct flock* arg = va_arg(va, struct flock*);
            va_end(va);
            fcntl_f(fd, cmd, arg);
        }
        break;
        case F_GETOWN_EX:
        case F_SETOWN_EX:
        {
            struct f_owner_exlock* arg = va_arg(va, struct f_owner_exlock*);
            va_end(va);
            return fcntl_f(fd, cmd, arg);
        }
        break;
        default:
            va_end(va);
            return fcntl_f(fd, cmd);
    }
    return -1;
}
// int ioctl_fun(int d, int request, ...){
//     va_list va;
//     va_start(va, request);
//     void* arg = va_arg(va, void*);
//     va_end(va);
//     if(FIONBIO == request){
//         bool usr_nonblock = !!*(int*)arg;
//          tw::FdCtx::ptr ctx = tw::FdMgr::GetInstance()->get(d);
//         if(!ctx || ctx->isClosed() || !ctx->isSocket()){
//             return ioctl(d, request, arg);
//         }
//         ctx->setUsrNonBlock(usr_nonblock);
//     }
//     return ioctl(d, request, arg);
// }

int getsockopt(int sockfd, int level, int optname,
                      void *optval, socklen_t *optlen){
    return getsockopt_f(sockfd, level, optname, optval, optlen);
}

int setsockopt(int sockfd, int level, int optname,
                      const void *optval, socklen_t optlen){
    if(tw::is_hook_enable()){
        return setsockopt_f(sockfd, level, optname, optval, optlen);
    }
    if(SOL_SOCKET == level){
        if(optname == SO_RCVTIMEO || optname == SO_SNDTIMEO){
            tw::FdCtx::ptr ctx = tw::FdMgr::GetInstance()->get(sockfd);
            if(ctx){
                const timeval* v = (const timeval*)optval;
                ctx->setTimeout(optname, v->tv_sec * 1000 + v->tv_usec / 1000);
            }
        }
    }
    return setsockopt_f(sockfd, level, optname, optval, optlen);
}
}