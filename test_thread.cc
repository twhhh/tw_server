#include "tw/log.h"
#include "tw/thread.h"
#include <iostream>
tw::Mutex m_mutex;
using MutexType = tw::Mutex;
tw::Logger::ptr m_logger = TW_LOG_ROOT();
void print(int i){
    MutexType::Lock lock(m_mutex);
    TW_LOG_INFO(m_logger) << "XXXXXXXXXX" << tw::Thread::GetName();

    sleep(1);
}

void test(){
    TW_LOG_INFO(m_logger) << "start";
    std::vector<tw::Thread::ptr> vecs;
    for(int i = 0; i < 4; ++i){
        tw::Thread::ptr thr(new tw::Thread(std::bind(&print, i), "name_" + std::to_string(i)));
        vecs.push_back(thr);
    }
    sleep(5);
    for(size_t i; i < vecs.size(); ++i){
        vecs[i]->join();
    }
    TW_LOG_INFO(m_logger) << "end";
}
int main(){
    test();
    return 0;
}