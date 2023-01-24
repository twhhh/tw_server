#pragma once
#include <ucontext.h>
#include <memory>
#include <functional>
namespace tw{
class Scheduler;
class Fiber : public std::enable_shared_from_this<Fiber>{
friend class Scheduler;
public:
    using ptr = std::shared_ptr<Fiber>;
    enum State{
        INIT,
        HOLD,
        EXEC,
        TERM,
        READY,
        EXECEPT
    };
private:
    Fiber();
public:
    Fiber(std::function<void()> cb, bool use_caller = false, size_t stackSize = 32 * 1024);
    ~Fiber();
    uint64_t getFiberID() const {return m_fiberID;}
    State getState() const {return m_state;}
    void setState(Fiber::State state) {m_state = state;}
    void swapIn();
    void swapOut();
    void call();
    void back();
    void reset(std::function<void()> cb);
    static Fiber::ptr GetThis();
    static void SetThis(Fiber* f);
    static void ReturnScedulerFunc();
    static void ReturnMainFunc();
    static uint64_t TotalFibers();
    static uint64_t GetFiberID();
    static void YieldToHold();

private:
    std::function<void()> m_cb;
    ucontext_t m_context;
    uint64_t m_fiberID = 0;
    void* m_stack = nullptr;
    uint32_t m_stackSize = 0;
    State m_state = INIT;
};
}