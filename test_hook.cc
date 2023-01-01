#include "tw/log.h"
#include "tw/hook.h"
#include "tw/iomanager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
static tw::Logger::ptr m_logger = TW_LOG_ROOT();
void test(){
    tw::IOManager iom("aa", 2, true);
    TW_LOG_INFO(m_logger) << "schedule start";
    iom.schedule([](){
        sleep(2);
        TW_LOG_INFO(m_logger) << "sleep2";
    });
    iom.schedule([](){
        sleep(3);
        TW_LOG_INFO(m_logger) << "sleep3";
    });
     TW_LOG_INFO(m_logger) << "schedule end";
}

void test_sock(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
    TW_LOG_INFO(m_logger) << "connect begin" << sock;
    int rt = connect(sock, (const sockaddr*)&addr, sizeof(addr));
    TW_LOG_INFO(m_logger) << "connect after" << rt << " errno=" << errno;
    if(rt){return;}
    const char data[] = "GET / HTTP1.0\r\n\r\n";
    rt = send(sock, data, sizeof(data), 0);
    TW_LOG_INFO(m_logger) << "send rt=" << rt << " errno=" << errno;
    if(rt <= 0){
        return;
    }
    std::string buf;
    buf.resize(4096);
    rt = recv(sock, &buf[0], buf.size(), 0);
    TW_LOG_INFO(m_logger) << "recv rt=" << rt << " errno=" << errno;
    if(rt <= 0){
        return;
    }
    buf.resize(rt);
    TW_LOG_INFO(m_logger) << buf;
}
int main(){
    //test();
    tw::IOManager iom;
    iom.schedule(test_sock);
    return 0;
}