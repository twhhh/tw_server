#include "iomanager.h"
#include "macro.h"
#include "log.h"

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
namespace tw{
static Logger::ptr m_logger = TW_LOG_NAME("system"); 
IOManager::FdContext::EventContext& IOManager::FdContext::getContext(Event ev){
    switch(ev){
        case Event::READ:
            return read;
        case Event::WRITE:
            return write;
        default:
            TW_ASSERT2(false, "getcontext");
    }
}
void IOManager::FdContext::resetContext(EventContext& ctx){
    ctx.scheduler = nullptr;
    ctx.fiber->reset(nullptr);
    ctx.cb = nullptr;
}

void IOManager::FdContext::triggerEvent(Event ev){
    //TW_LOG_DEBUG(m_logger) << "trigger";
    TW_ASSERT(ev & events)
    events = (Event)(events & ~ev);
    EventContext& ctx = getContext(ev);
    if(ctx.fiber){
        ctx.scheduler->schedule(&ctx.fiber);
    }else{
        ctx.scheduler->schedule(&ctx.cb);
    }
    ctx.scheduler = nullptr;
    return;
}

IOManager::IOManager(const std::string& name, size_t threadCount, bool use_caller)
    :Scheduler(name, threadCount, use_caller){
    m_epfd = epoll_create(1);
    TW_ASSERT(m_epfd > 0);
    int rt = pipe(m_tickleFds);
    TW_ASSERT(!rt);
    rt = fcntl(m_tickleFds[0], F_SETFL, O_NONBLOCK);
    TW_ASSERT(!rt);
    epoll_event event;
    memset(&event, 0, sizeof(epoll_event));
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = m_tickleFds[0];
    rt = epoll_ctl(m_epfd, EPOLL_CTL_ADD, m_tickleFds[0], &event);
    TW_ASSERT(!rt);
    fdContextResize(32);
    start();
}
IOManager::~IOManager(){
    stop();
    close(m_epfd);
    close(m_tickleFds[0]);
    close(m_tickleFds[1]);

    for(size_t i = 0; i < m_fdContexts.size(); ++i){
        if(m_fdContexts[i]){
            delete(m_fdContexts[i]);
        }
    }
}
// 加读锁
// 判断fd在不在m_fdContext里
// 去读锁
// 加fdContext锁
// 判ev和events关系
// 改events
// 确定op
// 确定新epoll_event
// epoll_ctl
// 改fdcontext

int IOManager::addEvent(int fd, Event ev, std::function<void()> cb){
    MutexType::ReadLock lock(m_mutex);
    FdContext* fd_ctx = nullptr;
    if((int)m_fdContexts.size() > fd){
        fd_ctx = m_fdContexts[fd];
        lock.unlock();
    }else{
        lock.unlock();
        MutexType::WriteLock lock2(m_mutex);
        fdContextResize(fd * 1.5);
        fd_ctx = m_fdContexts[fd];
    }
    FdContext::MutexType::Lock lock2(fd_ctx->mutex);
    if(fd_ctx->events & ev){
        TW_LOG_ERROR(m_logger) << "addEvent fd = " 
            << fd << " event = " << ev
            << " fd_ctx->events = " << fd_ctx->events;
        TW_ASSERT(false);
    }
    int op = fd_ctx->events ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
    epoll_event ep_event;
    memset(&ep_event, 0, sizeof(epoll_event));
    ep_event.events = EPOLLET | fd_ctx->events | ev;
    ep_event.data.ptr = fd_ctx;
    int rt = epoll_ctl(m_epfd, op, fd, &ep_event);
    if(rt){
        TW_LOG_ERROR(m_logger) << "epoll_ctl error fd = "
        << fd << " event = " << ev << " op = " << op 
        << rt << "(" << errno << ") (" << strerror(errno) << ")"; 
        return -1;
    }
    ++a_pendingEventCount;
    fd_ctx->events = (Event)(fd_ctx->events | ev);
    FdContext::EventContext& ctx = fd_ctx->getContext(ev);
    ctx.scheduler = Scheduler::GetThis();
    if(cb){
        ctx.cb.swap(cb);
    } else {
        ctx.fiber = Fiber::GetThis();
        TW_ASSERT2(ctx.fiber->getState() == Fiber::EXEC
        , "state = " << ctx.fiber->getState());
    }
    return 0;
}
bool IOManager::delEvent(int fd, Event ev){
    MutexType::ReadLock lock(m_mutex);
    if(fd >= (int)m_fdContexts.size()){
        return false;
    }
    FdContext* fd_ctx = m_fdContexts[fd];
    lock.unlock();
    FdContext::MutexType::Lock lock2(fd_ctx->mutex);
    if(!(fd_ctx->events & ev)){
        return false;
    }
    fd_ctx->events = (Event)(fd_ctx->events & ~ev);
    int op = fd_ctx->events ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
    epoll_event ep_event;
    memset(&ep_event, 0, sizeof(epoll_event));
    ep_event.events = EPOLLET | fd_ctx->events;
    ep_event.data.ptr = fd_ctx;
    int rt = epoll_ctl(m_epfd, op, fd, &ep_event);
    if(rt){
        TW_LOG_ERROR(m_logger) << "epoll_ctl error fd = "
        << fd << " event = " << ev << " op = " << op 
        << rt << "(" << errno << ") (" << strerror(errno) << ")"; 
        return false;
    }
    --a_pendingEventCount;
    FdContext::EventContext& ctx = fd_ctx->getContext(ev);
    fd_ctx->resetContext(ctx);
    return true;   
}
bool IOManager::cancelEvent(int fd, Event ev){
    MutexType::ReadLock lock(m_mutex);
    if(fd >= (int)m_fdContexts.size()){
        return false;
    }
    FdContext* fd_ctx = m_fdContexts[fd];
    lock.unlock();
    FdContext::MutexType::Lock lock2(fd_ctx->mutex);
    if(!(fd_ctx->events & ev)){
        return false;
    }
    Event new_event = (Event)(fd_ctx->events & ~ev);
    int op = new_event ? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
    epoll_event ep_event;
    memset(&ep_event, 0, sizeof(epoll_event));
    ep_event.events = EPOLLET | new_event;
    ep_event.data.ptr = fd_ctx;
    int rt = epoll_ctl(m_epfd, op, fd, &ep_event);
    if(rt){
        TW_LOG_ERROR(m_logger) << "epoll_ctl error fd = "
        << fd << " event = " << ev << " op = " << op 
        << rt << "(" << errno << ") (" << strerror(errno) << ")"; 
        return false;
    }
    fd_ctx->triggerEvent(ev);
    --a_pendingEventCount;
    return true;   
}
bool IOManager::cancelAll(int fd){
    if(!cancelEvent(fd, Event::READ)){
        return false;
    }
    if(!cancelEvent(fd, Event::WRITE)){
        return false;
    }
    return true;
}
void IOManager::fdContextResize(size_t size){
    if(size < m_fdContexts.size()){
        return;
    }
    m_fdContexts.resize(size);
    for(size_t i = 0; i < m_fdContexts.size(); ++i){
        if(!m_fdContexts[i]){
            m_fdContexts[i] = new FdContext;
            m_fdContexts[i]->fd = i;
        }
    }
}
IOManager* IOManager::GetThis(){
    return dynamic_cast<IOManager*>(Scheduler::GetThis());
}

void IOManager::tickle(){
    if(hasIdleThread()){
        return;
    }
    int rt = write(m_tickleFds[1], "t", 1);
    TW_ASSERT(rt == 1);
    TW_LOG_INFO(m_logger) << "tickle";
}
void IOManager::idle(){
    TW_LOG_INFO(m_logger) << "idle";
    epoll_event* events = new epoll_event[64]();
    std::shared_ptr<epoll_event> shared_events(events, 
        [](epoll_event* ptr){delete[] ptr;});
    while(true){
        uint64_t nexttime = 0;
        if(stopping(nexttime)){
            TW_LOG_INFO(m_logger) << "idle quit";
            break;
        }
        int rt = 0;
        do{
            static int MAX_TIMEOUT = 3000;
            if(nexttime != ~0ull){
                nexttime = (int)nexttime > MAX_TIMEOUT ? MAX_TIMEOUT : nexttime;
            }else{
                nexttime = MAX_TIMEOUT;
            }
            rt = epoll_wait(m_epfd, events, 64, (int)nexttime);
            if(rt < 0 && errno == EINTR){
            } else {
                break;
            }
        } while(true);
        std::vector<std::function<void()> > cbs;
        listExpiredCb(cbs);
        if(!cbs.empty()){
            schedule(cbs.begin(), cbs.end());
            cbs.clear();
        }
        for(int i = 0; i < rt; ++i){
            epoll_event event = events[i];
            if(event.data.fd == m_tickleFds[0]){
                uint8_t buf;
                while(read(m_tickleFds[0], &buf, 1) == 1);
                continue;
            }
            FdContext* fd_ctx = (FdContext*)event.data.ptr;
            FdContext::MutexType::Lock lock(fd_ctx->mutex);
            if(event.events & (EPOLLERR | EPOLLHUP)){
                if(fd_ctx->events & Event::READ){
                    lock.unlock();
                    delEvent(fd_ctx->fd, Event::READ);
                }
                if(fd_ctx->events & Event::WRITE){
                    lock.unlock();
                    delEvent(fd_ctx->fd, Event::WRITE);
                }
                lock.lock();
            }
            if(fd_ctx->events & Event::READ){
                lock.unlock();
                cancelEvent(fd_ctx->fd, Event::READ);
            }
            if(fd_ctx->events & Event::WRITE){
                lock.unlock();
                cancelEvent(fd_ctx->fd, Event::WRITE);
            }
        }
        Fiber::ptr cur = Fiber::GetThis();
        auto raw_ptr = cur.get();
        cur.reset();
        raw_ptr->swapOut();
    }
}
bool IOManager::stopping(){
    uint64_t timeout = 0;
    return stopping(timeout);
}
bool IOManager::stopping(uint64_t& timeout){
    timeout = getNextTime();
    return timeout == ~0ull
    && a_pendingEventCount == 0 
    && Scheduler::stopping();
}
void IOManager::addTimerFront(){
    tickle();
}
}