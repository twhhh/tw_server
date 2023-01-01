#include "fd_manager.h"
#include "hook.h"
#include "macro.h"
#include <sys/types.h>
#include <sys/stat.h>
namespace tw{
static tw::Logger::ptr m_logger = TW_LOG_NAME("system");
FdCtx::FdCtx(int fd)
    :m_isInit(false)
    ,m_isSocket(false)
    ,m_sysNonBlock(false)
    ,m_usrNonBlock(false)
    ,m_isClosed(false)
    ,m_fd(fd)
    ,m_recvTimeout(-1)
    ,m_sendTimeout(-1){
    init();
}
FdCtx::~FdCtx(){
}
bool FdCtx::init(){
    if(isInit()){
        return true;
    }
    m_recvTimeout = -1;
    m_sendTimeout = -1;
    struct stat fd_stat;
    if(-1 == fstat(m_fd, &fd_stat)){
        m_isInit = false;
        m_isSocket = false;
    }else{
        m_isInit = true;
        m_isSocket = S_ISSOCK(fd_stat.st_mode);
    }
    if(m_isSocket){
        int flags = fcntl(m_fd, F_GETFL, 0);
        if(!(flags & O_NONBLOCK)){
            TW_LOG_DEBUG(m_logger) << "fcntl";
            fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);
        }
        m_sysNonBlock = true;
    } else {
        m_sysNonBlock = false;
    }
    m_usrNonBlock = false;
    m_isClosed = false;
    return m_isInit;
}

uint64_t FdCtx::getTimeout(int type) const {
    if(type == SO_RCVTIMEO){
        return m_recvTimeout;
    } else {
        return m_sendTimeout;
    }
}

void FdCtx::setTimeout(int type, uint64_t to){
    if(type == SO_RCVTIMEO){
        m_recvTimeout = to;
    } else {
        m_sendTimeout = to;
    }
}

FdManager::FdManager(){
    m_fds.resize(64);
}
FdCtx::ptr FdManager::get(int fd, bool auto_create){
  if(fd == -1) {
        return nullptr;
    }
    MutexType::ReadLock lock(m_mutex);
    if((int)m_fds.size() <= fd) {
        if(auto_create == false) {
            return nullptr;
        }
    } else {
        if(m_fds[fd] || !auto_create) {
            return m_fds[fd];
        }
    }
    lock.unlock();

    MutexType::WriteLock lock2(m_mutex);
    FdCtx::ptr ctx(new FdCtx(fd));
    if(fd >= (int)m_fds.size()) {
        m_fds.resize(fd * 1.5);
    }
    m_fds[fd] = ctx;
    return ctx;
}
void FdManager::del(int fd){
    MutexType::WriteLock lock(m_mutex);
    if(fd >= (int)m_fds.size()){
        return;
    }
    m_fds[fd].reset();   
}
}