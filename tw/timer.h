#pragma once
#include <set>
#include <memory>
#include <functional>
#include <vector>
#include "thread.h"
namespace tw{
class TimerManager;
class Timer : public std::enable_shared_from_this<Timer>{
friend TimerManager;
public:
    using ptr = std::shared_ptr<Timer>;
    bool cancel();
    bool refresh();
    bool reset(uint64_t ms, bool from_now);
private:
    Timer(uint64_t ms, std::function<void()> cb
        , bool recurring, TimerManager* mgr);
    Timer(uint64_t next);
private:
    bool m_recurring = false;
    uint64_t m_ms = 0;
    uint64_t m_next = 0;
    std::function<void()> m_cb; 
    TimerManager* m_manager;
private:
    struct Comparator{
        bool operator()(const Timer::ptr& lhs, const Timer::ptr& rhs);
    };
};

class TimerManager{
friend Timer;
public:
    using ptr = std::shared_ptr<TimerManager>;
    using MutexType = RWMutex;
    TimerManager();
    virtual ~TimerManager();
    Timer::ptr addTimer(uint64_t ms, std::function<void()> cb, bool recurring =false);
    Timer::ptr addConditionTimer(uint64_t ms, std::function<void()> cb
        , std::weak_ptr<void> weak_cond, bool recurring = false);
    uint64_t getNextTime();
    void listExpiredCb(std::vector<std::function<void()> >& cbs);
    bool hasTimer();
protected:
    void addTimer(Timer::ptr timer, MutexType::WriteLock& lock);
    virtual void addTimerFront() = 0;
private:
    void updatePreviousTime(uint64_t ms);
private:
    MutexType m_mutex;
    std::set<Timer::ptr, Timer::Comparator> m_timers;
    uint64_t m_previousTime = 0;
    bool m_flag = false;//防止addTimer重复触发addTimerFront；

};
}