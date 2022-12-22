#pragma once
#include <pthread.h>
#include <thread>
#include <semaphore.h>
#include <functional>
#include <memory>
#include <string>

#include "Noncopyable.h"
namespace tw{
class Semaphore{
public:
    Semaphore(uint32_t n = 0);
    ~Semaphore();
    void notify();
    void wait();
private:
    sem_t m_sem;
};

class Thread : Noncopyable{
public:
    using ptr = std::shared_ptr<Thread>;
    Thread(std::function<void()> cb, const std::string& name);
    ~Thread();
    const std::string& getName() const {return m_name;}
    pid_t getID() const {return m_id;}
    void join();
    static Thread* GetThis();
    static const std::string& GetName();
    static void SetName(const std::string& name);
private:
    static void* run(void* arg);
private:
    pid_t m_id = -1;
    pthread_t m_thread;
    std::function<void()> m_cb;
    std::string m_name;
    Semaphore m_sem;
};

template<class T>
class ScopedLockImpl{
public:
    ScopedLockImpl(T& mutex)
        :m_mutex(mutex){
        m_mutex.lock();
        m_locked = true;
    }
    ~ScopedLockImpl(){
        unlock();
    }
    void lock(){
        if(!m_locked){
            m_mutex.lock();
            m_locked = true;
        }
    }
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};
template<class T>
class ReadScopedLockImpl{
public:
    ReadScopedLockImpl(T& mutex)
        :m_mutex(mutex){
        m_mutex.rdlock();
        m_locked = true;
    }
    ~ReadScopedLockImpl(){
        unlock();
    }
    void lock(){
        if(!m_locked){
            m_mutex.rdlock();
            m_locked = true;
        }
    }
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;

};

template<class T>
class WriteScopedLockImpl{
public:
    WriteScopedLockImpl(T& mutex)
        :m_mutex(mutex){
        m_mutex.wrlock();
        m_locked = true;
    }
    ~WriteScopedLockImpl(){
        unlock();
    }
    void lock(){
        if(!m_locked){
            m_mutex.wrlock();
            m_locked = true;
        }
    }
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;

};
class Mutex : Noncopyable{
public:
    using Lock = ScopedLockImpl<Mutex>;
    Mutex(){
        pthread_mutex_init(&m_mutex, nullptr);
    }
    ~Mutex(){
        pthread_mutex_destroy(&m_mutex);
    }
    void lock(){
        pthread_mutex_lock(&m_mutex);
    }
    void unlock(){
        pthread_mutex_unlock(&m_mutex);
    }
private:
    pthread_mutex_t m_mutex;
};

class SpinMutex : Noncopyable{
public:
    using Lock = ScopedLockImpl<SpinMutex>;
    SpinMutex(){
        pthread_spin_init(&m_mutex, 0);
    }
    ~SpinMutex(){
        pthread_spin_destroy(&m_mutex);
    }
    void lock(){
        pthread_spin_lock(&m_mutex);
    }
    void unlock(){
        pthread_spin_unlock(&m_mutex);
    }
private:
    pthread_spinlock_t m_mutex;
};
class NullMutex : Noncopyable{
public:
    using Lock = ScopedLockImpl<NullMutex>;
    NullMutex(){}
    ~NullMutex(){}
    void lock(){}
    void unlock(){}
};
class RWMutex : Noncopyable{
public:
    using ReadLock = ReadScopedLockImpl<RWMutex>;
    using WriteLock = WriteScopedLockImpl<RWMutex>;
    RWMutex(){
        pthread_rwlock_init(&m_mutex, nullptr);
    }
    ~RWMutex(){
        pthread_rwlock_destroy(&m_mutex);
    }
    void rdlock(){
        pthread_rwlock_rdlock(&m_mutex);
    }
    void wrlock(){
        pthread_rwlock_wrlock(&m_mutex);
    }
    void unlock(){
        pthread_rwlock_unlock(&m_mutex);
    }
private:
    pthread_rwlock_t m_mutex;
};

class NullRWMutex : Noncopyable{
public:
    using ReadLock = ReadScopedLockImpl<NullRWMutex>;
    using WriteLock = WriteScopedLockImpl<NullRWMutex>;
    NullRWMutex(){}
    ~NullRWMutex(){}
    void rdlock(){}
    void wrlock(){}
    void unlock(){}
};

}