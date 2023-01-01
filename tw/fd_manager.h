#pragma once
#include <fcntl.h>
#include <unistd.h>
#include <memory>
#include <vector>
#include "thread.h"
#include "singleton.h"
namespace tw{
class FdCtx : public std::enable_shared_from_this<FdCtx>{
public:
    using ptr = std::shared_ptr<FdCtx>;
    FdCtx(int fd);
    ~FdCtx();
    bool isInit() const {return m_isInit;}
    bool isSocket() const {return m_isSocket;}
    bool isClosed() const {return m_isClosed;}
    bool getSysNonBlock() const {return m_sysNonBlock;}
    bool getUsrNonBlock() const {return m_usrNonBlock;}
    // type: 读超时 SO_RCVTIMEO
    //       写超时 SO_SNDTIMEO
    uint64_t getTimeout(int type) const;
    void setSysNonBlock(bool v) {m_sysNonBlock = v;}
    void setUsrNonBlock(bool v) {m_usrNonBlock = v;}
    void setTimeout(int type, uint64_t to);
private:
    bool init();
private:
    bool m_isInit : 1;
    bool m_isSocket : 1;
    bool m_sysNonBlock : 1;
    bool m_usrNonBlock : 1;
    bool m_isClosed : 1;
    int m_fd;
    uint64_t m_recvTimeout;
    uint64_t m_sendTimeout;
};

class FdManager{
public:
    //using ptr = std::shared_ptr<FdManager>;
    using MutexType = RWMutex;
    FdManager();
    FdCtx::ptr get(int fd, bool auto_create = false);
    void del(int fd);
private:
    MutexType m_mutex;
    std::vector<FdCtx::ptr> m_fds;
};

using FdMgr = Singleton<FdManager>;
}