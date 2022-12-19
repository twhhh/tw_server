#pragma once
#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <list>
#include <atomic>

#include "fiber.h"
#include "thread.h"
namespace tw{
class Scheduler{
public:
    using ptr = std::shared_ptr<Scheduler>;
    using MutexType = Mutex;
    Scheduler(const std::string& name = "", size_t threadCount = 1, bool use_caller = true);
    void start();
    void stop();
    static Fiber* GetSchedulerFiber();
    static Scheduler* GetThis();
    template<class FiberOrFunc>
    void schedule(FiberOrFunc f){
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            need_tickle = scheduleNoLock(f);
        }
        if(need_tickle){
            tickle();
        }
    }
    template<class Iterator>
    void schedule(Iterator begin, Iterator end){
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            while(begin != end){
                need_tickle = scheduleNoLock(*begin) || need_tickle;//
                ++begin;
            }
        }
        if(need_tickle){
            tickle();
        }
    }
private:
    template<class FiberOrFunc>
    bool scheduleNoLock(FiberOrFunc f){
        bool need_tickle = m_tasks.empty();
        FiberAndFunc ff(f);
        if(ff.cb || ff.fiber){
            m_tasks.push_back(ff);
        }
        return need_tickle;
    }
protected:
    virtual void tickle();
    virtual void idle();
    void run();
    void setThis();
    bool stopping();
private:
    struct FiberAndFunc{
        Fiber::ptr fiber = nullptr;
        std::function<void()> cb = nullptr;
        FiberAndFunc(Fiber::ptr f):fiber(f){}
        FiberAndFunc(std::function<void()> c):cb(c){}
        FiberAndFunc(){}
        void reset(){
            fiber = nullptr;
            cb = nullptr;
        }
    };
private:
    MutexType m_mutex;
    std::string m_name;
    std::vector<Thread::ptr> m_threads;//threads_pool
    std::list<FiberAndFunc> m_tasks;//why list? because fcfs
    size_t m_threadCount = 0;
    Fiber::ptr m_scheduleFiber;
protected:
    int m_mainThread = 0;
    std::vector<int> m_threadIDs;
    std::atomic<size_t> a_activeThreadCount{0};
    std::atomic<size_t> a_idleThreadCount{0};
    bool m_stopping = false;
    bool m_autostop = false;
};
}