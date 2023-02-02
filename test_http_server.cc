#include "tw/log.h"
#include "tw/http/http_server.h"
static tw::Logger::ptr s_logger = TW_LOG_ROOT();

void run(){
    tw::http::HttpServer::ptr server(new tw::http::HttpServer);
    auto addr = tw::IPv4Address::Create("0.0.0.0", 8020);
    while(!server->bind(addr)){
        sleep(2);
    }
    server->start();
}
int main(int argc, char** argv){
    tw::IOManager iom("tw", 2);
    iom.schedule(run);
    return 0;
}