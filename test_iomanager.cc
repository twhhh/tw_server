#include "tw/iomanager.h"
#include "tw/log.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>

static tw::Logger::ptr m_logger = TW_LOG_ROOT();
int sock = 0;
void test(){
    TW_LOG_INFO(m_logger) << "XXXXXXXXXXXXXXXXxx";
}
void test02(){
    sock = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "180.101.49.13", &addr.sin_addr.s_addr);
    if(!connect(sock, (const sockaddr*)&addr, sizeof(addr))){

    }else if(errno == EINPROGRESS){
         TW_LOG_INFO(m_logger) << "add event errno=" << errno << " " << strerror(errno);
        tw::IOManager::GetThis()->addEvent(sock,tw::IOManager::READ
        , [](){
            TW_LOG_INFO(m_logger) << "read callback";
             });
        tw::IOManager::GetThis()->addEvent(sock,tw::IOManager::WRITE
        , [](){
            TW_LOG_INFO(m_logger) << "write callback";
            tw::IOManager::GetThis()->cancelEvent(sock, tw::IOManager::READ);
            close(sock);
             });
        
    }else{
        TW_LOG_INFO(m_logger) << "errno" << errno << strerror(errno);
    }
    tw::IOManager::GetThis()->cancelEvent(sock, tw::IOManager::WRITE);
}
void test01(){
    tw::IOManager iom("n", 2, false);
    iom.schedule(&test02);
}
tw::Timer::ptr s_timer;
void test_3(){
    tw::IOManager iom("n", 2, true);
    iom.addTimer(1500, [](){
        TW_LOG_INFO(m_logger) << "1500";
    });
    s_timer = iom.addTimer(1000, [](){
        static int i = 0;
        TW_LOG_INFO(m_logger) << "test addTimer" << i;
    }, true);

}
int main(){
    //test01();
    test_3();
    return 0;
}