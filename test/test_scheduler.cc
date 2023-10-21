#include "tw/scheduler.h"
#include "tw/log.h"

static tw::Logger::ptr m_logger = TW_LOG_ROOT();
void print(){
    TW_LOG_INFO(m_logger) << "XXXXXXXXXXXXXXXXXXx";
}
void test_scheduler(){
    tw::Scheduler::ptr m_sc(new tw::Scheduler("sha", 2, false));
    for(int i = 0; i < 10 ; i++){
        m_sc->schedule(&print);
    }
    m_sc->start();
    sleep(10);
    m_sc->stop();
}
int main(){
    test_scheduler();
    return 0;
}