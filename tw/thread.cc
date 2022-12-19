#include "thread.h"
#include "log.h"
#include <stdexcept>
namespace tw{
static thread_local Thread* t_thread = nullptr;
static thread_local std::string t_thread_name = "UNKNOW";
static Logger::ptr m_logger = TW_LOG_NAME("system");
Semaphore::Semaphore(uint32_t n){
    if(sem_init(&m_sem, 0, n) ){
        throw std::logic_error("sem_init error");
    }
}
Semaphore::~Semaphore(){
    if(sem_destroy(&m_sem)){
        throw std::logic_error("sem_destroy error");
    }
}
void Semaphore::notify(){
    if(sem_post(&m_sem)){
        throw std::logic_error("sem_post error");
    }
}
void Semaphore::wait(){
    if(sem_wait(&m_sem)){
        throw std::logic_error("sem_wait error");
    }
}
Thread::Thread(std::function<void()> cb, const std::string& name)
    :m_cb(cb), m_name(name){
    if(name.empty()){
        m_name = "UNKNOW";
    }
    int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
    if(rt){
        TW_LOG_ERROR(m_logger) << "pthread_create failed , name = " 
            << name;
        throw std::logic_error("pthread_create failed");
    }
    m_sem.wait();
}

Thread::~Thread(){
    if(m_thread){
        pthread_detach(m_thread);
    }
}

void Thread::join(){
    int rt = pthread_join(m_thread, nullptr);
    if(rt){
        TW_LOG_ERROR(m_logger) << "pthread_join failed  ";
        throw std::logic_error("pthread_error failed");
    }
    m_thread = 0;
}
void* Thread::run(void* arg){
    Thread* thread = (Thread*)arg;
    t_thread = thread;
    t_thread_name = thread->m_name;
    thread->m_id = tw::GetThreadID();
    pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());
    std::function<void()> cb;
    cb.swap(thread->m_cb);
    thread->m_sem.notify();
    cb();
    return 0;
}
Thread* Thread::GetThis(){
    return t_thread;
}
const std::string& Thread::GetName(){
    return t_thread_name;
}
void Thread::SetName(const std::string& name){
    if(t_thread){
        t_thread->m_name = name;
    }
    t_thread_name = name;
}
}