#include "timer.h"
#include "util.h"
#include "log.h"
namespace tw{
static Logger::ptr m_logger = TW_LOG_NAME("system");
bool Timer::Comparator::operator()(const Timer::ptr& lhs, const Timer::ptr& rhs){
    if(!lhs && !rhs){
        return false;
    }
    if(!lhs){
        return true;
    }
    if(!rhs){
        return false;
    }
    if(lhs->m_next < rhs->m_next){
        return true;
    }
    if(lhs->m_next > rhs->m_next){
        return false;
    }
    return lhs.get() < rhs.get();
}

Timer::Timer(uint64_t ms, std::function<void()> cb
        , bool recurring, TimerManager* mgr)
        :m_recurring(recurring)
        ,m_ms(ms)
        ,m_cb(cb)
        ,m_manager(mgr){
    m_next = m_ms + GetCurrentMS();
}
Timer::Timer(uint64_t next)
    :m_next(next){
}
bool Timer::cancel(){
    TimerManager::MutexType::WriteLock lock(m_manager->m_mutex);
    if(m_cb){
        m_cb = nullptr;
        auto it = m_manager->m_timers.find(shared_from_this());
        m_manager->m_timers.erase(it);
        return true;
    }
    return false;
}
bool Timer::refresh(){
    TimerManager::MutexType::WriteLock lock(m_manager->m_mutex);
    if(!m_cb){
        return false;
    }
    auto it = m_manager->m_timers.find(shared_from_this());
    if(it == m_manager->m_timers.end()){
        return false;
    }
    m_manager->m_timers.erase(it);
    m_next = m_ms + GetCurrentMS();
    m_manager->m_timers.insert(shared_from_this());
    return true;
}
bool Timer::reset(uint64_t ms, bool from_now){
    TimerManager::MutexType::WriteLock lock(m_manager->m_mutex);
    if(!m_cb){
        return false;
    }
    auto it = m_manager->m_timers.find(shared_from_this());
    if(it == m_manager->m_timers.end()){
        return false;
    }
    m_manager->m_timers.erase(it);
    uint64_t new_start = 0;
    if(from_now){
        new_start = GetCurrentMS() + ms;
    }else{
        new_start = m_next - m_ms + ms;
    }
    m_ms = ms;
    m_next = new_start;
    m_manager->addTimer(shared_from_this(), lock);
    return true;
}

TimerManager::TimerManager(){
    m_previousTime = GetCurrentMS();
}
TimerManager::~TimerManager(){
}
Timer::ptr TimerManager::addTimer(uint64_t ms, std::function<void()> cb, bool recurring){
    std::shared_ptr<Timer> timer(new Timer(ms, cb, recurring, this));
    MutexType::WriteLock lock(m_mutex);
    addTimer(timer, lock);
    return timer;
}
static void OnTimer(std::function<void()> cb, std::weak_ptr<void> weak_cond){
    std::shared_ptr<void> tmp = weak_cond.lock();
    if(tmp){
        cb();
    }
}
Timer::ptr TimerManager::addConditionTimer(uint64_t ms, std::function<void()> cb
    , std::weak_ptr<void> weak_cond, bool recurring){
    return addTimer(ms, std::bind(&OnTimer, cb, weak_cond), recurring);
}
void TimerManager::addTimer(Timer::ptr timer, MutexType::WriteLock& lock){
    auto it = m_timers.insert(timer).first;
    bool atFrontFlag = (it == m_timers.begin()) && (!m_flag);
    if(atFrontFlag){
        m_flag = true;
    }
    lock.unlock();
    if(atFrontFlag){
        addTimerFront();
    }
}

uint64_t TimerManager::getNextTime(){
    RWMutex::ReadLock lock(m_mutex);
    m_flag = false;
    if(m_timers.empty()){
        return ~0ull;
    }
    const Timer::ptr& timer = *m_timers.begin();
    uint64_t now_ms = GetCurrentMS();
    if(now_ms >= timer->m_next){
        //TW_LOG_DEBUG(m_logger) << "test";
        return 0;
    }else{
        return timer->m_next - now_ms;
    }
}
void TimerManager::listExpiredCb(std::vector<std::function<void()> >& cbs){
    uint64_t now_ms = GetCurrentMS();
    std::vector<Timer::ptr> expired;
    {
        MutexType::ReadLock lock(m_mutex);
        if(m_timers.empty()){
            return;
        }
    }
    MutexType::WriteLock lock(m_mutex);
    if(m_timers.empty()){
        return;
    }
    updatePreviousTime(now_ms);
    std::shared_ptr<Timer> now_time(new Timer(now_ms));
    auto it = m_timers.lower_bound(now_time);
    while(it != m_timers.end() && (*it)->m_next == now_ms){
        ++it;
    }
    expired.insert(expired.begin(), m_timers.begin(), it);
    m_timers.erase(m_timers.begin(), it);
    cbs.reserve(expired.size());
    for(auto& timer : expired){
        cbs.push_back(timer->m_cb);
        if(timer->m_recurring){
            timer->m_next = now_ms + timer->m_ms;
            m_timers.insert(timer);
        }else{
            timer->m_cb = nullptr;
        }
    }
}
void TimerManager::updatePreviousTime(uint64_t ms){
    m_previousTime = ms;
}
}