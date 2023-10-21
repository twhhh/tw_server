#include "tw/log.h"
#include "tw/iomanager.h"
#include "tw/socket.h"
static tw::Logger::ptr s_logger = TW_LOG_ROOT();
void test00(){
    tw::IPAddress::ptr addr = tw::Address::LookupAnyIPAddress("www.baidu.com");
    if(addr){
        TW_LOG_INFO(s_logger) << "get address: " << addr->toString(); 
    }else{
        TW_LOG_ERROR(s_logger) << "get address fail";
        return;
    }
    tw::Socket::ptr sock = tw::Socket::CreateTCP(addr);
    addr->setPort(80);
    TW_LOG_INFO(s_logger) << "addr = " << addr->toString();
    if(!sock->connect(addr)){
        TW_LOG_ERROR(s_logger) << "connect: fail";
        return;
    } else {
        TW_LOG_INFO(s_logger) << "connect success";
    }
    const char buf[] = "GET / HTTP1.0\r\n\r\n";
    int rt = sock->send(buf, sizeof(buf));
    if(rt <= 0){
        TW_LOG_ERROR(s_logger) << "send fail rt = " << rt;
        return;
    }
    std::string buffs;
    buffs.resize(4096);
    rt = sock->recv(&buffs[0], buffs.size());
    if(rt <= 0){
        TW_LOG_ERROR(s_logger) << "recb fail rt = " << rt;
        return;
    }
    buffs.resize(rt);
    TW_LOG_INFO(s_logger) << buffs;
}
int main(){
    tw::IOManager iom;

    iom.schedule(&test00);
    return 0;
}