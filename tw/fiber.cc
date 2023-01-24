#include "fiber.h"
#include "log.h"
#include "macro.h"
#include "scheduler.h"
#include <thread>
#include <atomic>

namespace tw{
    static tw::Logger::ptr m_logger = TW_LOG_NAME("system");
    static thread_local Fiber* t_fiber = nullptr;
    static thread_local Fiber::ptr t_main_fiber = nullptr;
    static std::atomic<uint64_t> a_fiberID{0};
    static std::atomic<uint64_t> a_total_fibers{0};
    Fiber::Fiber(){
        m_state = EXEC;
        SetThis(this);
        if(getcontext(&m_context)){
            TW_ASSERT2(false, "getcontext");
        }
        ++a_total_fibers;
        TW_LOG_INFO(m_logger) << "Fiber()";
    }
    Fiber::Fiber(std::function<void()> cb, bool use_caller, size_t stackSize)
        :m_cb(cb), m_fiberID(++a_fiberID){
        ++a_total_fibers;
        m_stackSize = stackSize < 128 * 1024 ? stackSize : 128 * 1024;
        m_stack = (void*)malloc(m_stackSize);
        if(getcontext(&m_context)){
            TW_ASSERT2(false, "getcontext");
        }
        m_context.uc_link = nullptr;
        m_context.uc_stack.ss_size = m_stackSize;
        m_context.uc_stack.ss_sp = m_stack;
        if(use_caller){
            makecontext(&m_context, &Fiber::ReturnMainFunc, 0);
        }else{
            makecontext(&m_context, &Fiber::ReturnScedulerFunc, 0);
        }
        m_state = INIT;
        TW_LOG_INFO(m_logger) << "Fiber(cb) id = " << m_fiberID;
    }
    Fiber::~Fiber(){
        --a_total_fibers;
        if(m_stack){
            TW_ASSERT(m_state == INIT || m_state == TERM || m_state == EXECEPT);
            free(m_stack);
        }else{
            TW_ASSERT(!m_cb);
            TW_ASSERT(m_state == EXEC);//just is main fiber
            Fiber* cur = t_fiber;
            if(cur == this){
                t_fiber = nullptr;
                t_main_fiber = nullptr;
            }
        }
        TW_LOG_INFO(m_logger) << "~Fiber id = " << m_fiberID;
    }
    void Fiber::swapIn(){
        SetThis(this);
        m_state = EXEC;
        if(swapcontext(&Scheduler::GetSchedulerFiber()->m_context, &m_context)){
            TW_ASSERT2(false, "swapcontext");
        }
    }
    void Fiber::swapOut(){
        SetThis(Scheduler::GetSchedulerFiber());
        if(swapcontext(&m_context, &Scheduler::GetSchedulerFiber()->m_context)){
            TW_ASSERT2(false, "swapcontext");
        }
    }
    void Fiber::call(){
        SetThis(this);
        m_state = EXEC;
        TW_ASSERT(t_main_fiber);
        Fiber::ptr m = t_main_fiber;
        if(swapcontext(&m->m_context, &m_context)){
            TW_ASSERT2(false, "swapcontext");
        }
    }
    void Fiber::back(){
        TW_ASSERT(t_main_fiber);
        Fiber::ptr m = t_main_fiber;
        if(swapcontext(&m_context, &m->m_context)){
            TW_ASSERT2(false, "swapcontext");
        }
    }
    Fiber::ptr Fiber::GetThis(){
        if(t_fiber){
            return t_fiber->shared_from_this();
        }
        Fiber::ptr main_fiber(new Fiber);
        TW_ASSERT(t_fiber == main_fiber.get());
        t_main_fiber = main_fiber;
        return t_fiber->shared_from_this();
    }

    void Fiber::reset(std::function<void()> cb){
        TW_ASSERT(m_stack);
        TW_ASSERT(m_state == INIT || m_state == TERM || m_state == EXECEPT);
        m_cb = cb;
        if(getcontext(&m_context)){
            TW_ASSERT2(false, "getcontext");
        }
        m_context.uc_link = nullptr;
        m_context.uc_stack.ss_size = m_stackSize;
        m_context.uc_stack.ss_sp = m_stack;

        makecontext(&m_context, &Fiber::ReturnScedulerFunc, 0);
        m_state = INIT;
    }
    void Fiber::SetThis(Fiber* f){
        t_fiber = f;
    }
    void Fiber::ReturnScedulerFunc(){
        Fiber::ptr cur = GetThis();
        TW_ASSERT(cur);
        try{
            cur->m_cb();
            cur->m_cb = nullptr;
            cur->m_state = TERM;
        }catch (std::exception& ex){
            cur->m_state = EXECEPT;
            TW_LOG_ERROR(m_logger) << "Fiber except: " << ex.what()
                << "fiber id = " << cur->getFiberID()
                << std::endl
                << BackTraceToString();
        }catch(...){
            cur->m_state = EXECEPT;
            TW_LOG_ERROR(m_logger) << "Fiber except: "
            << "fiber id = " << cur->getFiberID()
            << std::endl
            << BackTraceToString();
        }
        auto raw_ptr = cur.get();
        cur.reset();
        raw_ptr->m_state = TERM;
        raw_ptr->swapOut();
        TW_ASSERT2(false, "never come here! fiber id = " << cur->getFiberID());
    }
    void Fiber::ReturnMainFunc(){
        Fiber::ptr cur = GetThis();
        TW_ASSERT(cur);
        try{
            cur->m_cb();
            cur->m_cb = nullptr;
            cur->m_state = TERM;
        }catch (std::exception& ex){
            cur->m_state = EXECEPT;
            TW_LOG_ERROR(m_logger) << "Fiber except: " << ex.what()
                << "fiber id = " << cur->getFiberID()
                << std::endl
                << BackTraceToString();
        }catch(...){
            cur->m_state = EXECEPT;
            TW_LOG_ERROR(m_logger) << "Fiber except: "
            << "fiber id = " << cur->getFiberID()
            << std::endl
            << BackTraceToString();
        }
        auto raw_ptr = cur.get();
        cur.reset();
        raw_ptr->m_state = TERM;
        raw_ptr->back();
        TW_ASSERT2(false, "never come here! fiber id = " << cur->getFiberID());
    }
    uint64_t Fiber::TotalFibers(){
        return a_total_fibers;
    }
    uint64_t Fiber::GetFiberID(){
        if(t_fiber){
            return t_fiber->getFiberID();
        }
        return 0;
    }
    void Fiber::YieldToHold(){
        Fiber::ptr cur = GetThis();
        TW_ASSERT(cur->getState() == Fiber::EXEC);
        //cur->m_state = Fiber::HOLD;
        cur->swapOut();
    }
}