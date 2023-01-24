#include "scheduler.h"
#include "log.h"
#include "macro.h"
#include "hook.h"

namespace tw{
static Logger::ptr m_logger = TW_LOG_NAME("system");
static thread_local Scheduler* t_scheduler = nullptr;
static thread_local Fiber*  t_scheduler_fiber = nullptr;
Scheduler::Scheduler(const std::string& name, size_t threadCount, bool use_caller)
    :m_name(name){
    TW_ASSERT(threadCount > 0);
    if(use_caller){
        m_threadCount = threadCount - 1;
        Fiber::GetThis();
        TW_ASSERT(t_scheduler == nullptr);
        t_scheduler = this;
        Thread::SetName(name);
        m_scheduleFiber.reset(new Fiber(std::bind(&Scheduler::run, this), true));
        t_scheduler_fiber = m_scheduleFiber.get();
        m_mainThread = GetThreadID();
        m_threadIDs.push_back(m_mainThread);
    }else{
        m_threadCount = threadCount;
        m_mainThread = 0;
    }
}
void Scheduler::start(){
    TW_LOG_INFO(m_logger) << "start";
    MutexType::Lock lock(m_mutex);
    TW_ASSERT(m_threads.empty());
    m_threads.resize(m_threadCount);
    for(size_t i = 0; i < m_threadCount; ++i){
        m_threads[i].reset(new Thread(std::bind(&Scheduler::run, this), 
            m_name + "_" + std::to_string(i)));
        m_threadIDs.push_back(m_threads[i]->getID());
    }
}
void Scheduler::stop(){
    TW_LOG_INFO(m_logger) << "stop";
    m_autostop = true;
    if(m_scheduleFiber
        && m_threadCount == 0
        && (m_scheduleFiber->getState() == Fiber::INIT || m_scheduleFiber->getState() == Fiber::TERM)){
        TW_LOG_INFO(m_logger) << this << "stopped";
        m_stopping = true;

        if(stopping()){
            return;
        }
    }
    m_stopping = true;
    if(m_mainThread){
        if(!stopping()){
            m_scheduleFiber->call();
        }
    }//start caller scheduler
    std::vector<Thread::ptr> thrs;
    {
        MutexType::Lock lock(m_mutex);
        thrs.swap(m_threads);
    }
    for(auto& i : thrs){
        i->join();
    }//quit
}

void Scheduler::run(){
    TW_LOG_INFO(m_logger) << "run ";
    set_hook_enable(true);
    setThis();
    if(GetThreadID() != m_mainThread){
        t_scheduler_fiber = Fiber::GetThis().get();
    }
    //  run    <-- idle
    //  fiber  --> fiber
    //  | ^ 
    //  v |
    //  event
    //  fibers
    Fiber::ptr idle(new Fiber(std::bind(&Scheduler::idle, this)));
    Fiber::ptr event;

    FiberAndFunc ff;
    while(true){
        ff.reset();
        //bool need_tickle = false;
        bool is_active = false;
        {
            MutexType::Lock lock(m_mutex);
            auto it = m_tasks.begin();
            while(it != m_tasks.end()){
                if(it->fiber && it->fiber->getState() == Fiber::EXEC){
                    ++it;
                    continue;
                }
                ff = *it;
                m_tasks.erase(it);
                ++a_activeThreadCount;
                is_active = true;
                break;
            }
        }//get one task
        if(ff.fiber && ff.fiber->getState() != Fiber::TERM
                && ff.fiber->getState() != Fiber::EXECEPT){
            ff.fiber->swapIn();
            --a_activeThreadCount;
            if(ff.fiber->getState() == Fiber::READY) {
                schedule(ff.fiber);
            } else if(ff.fiber->getState() != Fiber::TERM
                    && ff.fiber->getState() != Fiber::EXECEPT) {
                ff.fiber->m_state = Fiber::HOLD;
            }
            ff.reset();   
        } else if(ff.cb){
            if(event){
                event->reset(ff.cb);
            }else{
                event.reset(new Fiber(ff.cb));
            }
            ff.reset();
            event->swapIn();
            --a_activeThreadCount;
            // if(event->getState() == Fiber::HOLD){
            //    schedule(event);
            // }else 
            if(event->getState() == Fiber::TERM ||
                        event->getState() == Fiber::EXECEPT){
                event.reset();
            }else{
                event->m_state = Fiber::HOLD;
                event.reset();
            }
        }else {//finish a task
            if(is_active){
            --a_activeThreadCount;
            continue;
            }
            if(idle->getState() == Fiber::TERM){
                TW_LOG_INFO(m_logger) << "idle fiber term";
                break;
            }
            ++a_idleThreadCount;
            idle->swapIn();
            --a_idleThreadCount;
            if(idle->getState() != Fiber::TERM 
            && idle->getState() != Fiber::EXECEPT){
                idle->m_state = Fiber::HOLD;
            }
        }
    }

}

void Scheduler::setThis(){
    t_scheduler = this;
}

bool Scheduler::stopping(){
    MutexType::Lock lock(m_mutex);
    return m_autostop && m_stopping 
        && m_tasks.empty() && a_activeThreadCount == 0;
}
void Scheduler::tickle(){
    TW_LOG_INFO(m_logger) << "tickle";
}

void Scheduler::idle(){
    TW_LOG_INFO(m_logger) << "idle";
    while(!stopping()){
        Fiber::YieldToHold();
    }
}

Fiber* Scheduler::GetSchedulerFiber(){
    return t_scheduler_fiber;
}
Scheduler* Scheduler::GetThis(){
    return t_scheduler;
}
}