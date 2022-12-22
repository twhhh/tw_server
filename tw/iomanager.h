#pragma once
#include <vector>
#include <sys/epoll.h>
#include "scheduler.h"
#include "thread.h"
#include "timer.h"
namespace tw{
class IOManager : public Scheduler, public TimerManager{
public:
    using ptr = std::shared_ptr<IOManager>;
    using MutexType = RWMutex;
    enum Event{
        NONE = 0x00,
        READ = 0x01, //EPOLL_IN
        WRITE = 0x04 //EPOLL_OUT
    };
private:
    struct FdContext{
        using MutexType = Mutex;
        struct EventContext{
            Scheduler* scheduler = nullptr;;
            Fiber::ptr fiber = nullptr;
            std::function<void()> cb = nullptr;
        };
        EventContext& getContext(Event ev);
        void resetContext(EventContext& ctx);
        void triggerEvent(Event ev);
        int fd;
        EventContext read;
        EventContext write;
        Event events = NONE;
        MutexType mutex;
    };
    void fdContextResize(size_t size);
public:
    IOManager(const std::string& name = "", size_t threadCount = 1, bool use_caller = true);
    ~IOManager();
    int addEvent(int fd, Event ev, std::function<void()> cb = nullptr);
    bool delEvent(int fd, Event ev);
    bool cancelEvent(int fd, Event ev);
    bool cancelAll(int fd);
    static IOManager* GetThis();
protected:
    void tickle()override;
    void idle()override;
    bool stopping()override;
    bool stopping(uint64_t& timeout);
    void addTimerFront() override;
    //timer
private:
    int m_epfd = 0;
    int m_tickleFds[2];
    MutexType m_mutex;
    std::vector<FdContext*> m_fdContexts;
    std::atomic<size_t> a_pendingEventCount = {0};
};
}