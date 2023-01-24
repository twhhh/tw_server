#include "../tw/address.h"
#include "../tw/tcp_server.h"
#include "../tw/log.h"
#include "../tw/bytearray.h"
#include "../tw/iomanager.h"
#include "../tw/socket.h"
static tw::Logger::ptr s_logger = TW_LOG_ROOT();
class EchoServer : public tw::TcpServer{
public:
    EchoServer(int type);
    void handleClient(tw::Socket::ptr client) override;
private:
    int m_type = 0;
};
EchoServer::EchoServer(int type)
    :m_type(type){
}
void EchoServer::handleClient(tw::Socket::ptr client){
    TW_LOG_INFO(s_logger) << "handleClient" << *client;
    tw::ByteArray::ptr ba(new tw::ByteArray);
    while(true){
        ba->clear();
        std::vector<iovec> iovs;
        ba->getWriteBuffers(iovs, 1024);
        int rt = client->recv(&iovs[0], iovs.size());
        if(rt == 0){
            TW_LOG_INFO(s_logger) << "client(" << *client
            << ") close";
            break;
        } else if(rt < 0){
            TW_LOG_INFO(s_logger) << "client errer rt = "
            << rt << "errno = " << errno << " strerr = "
            << strerror(errno);
            break;
        }
        ba->setPosition(ba->getPosition() + rt);
        ba->setPosition(0);
        if(m_type == 1){
            std::cout << ba->toString();
        } else {
            std::cout << ba->toHexString();
        }
        std::cout.flush();
    }
}
int type = 1;
void run(){
    TW_LOG_INFO(s_logger) << "server type=" << type;
    EchoServer::ptr es(new EchoServer(type));
    auto addr = tw::IPv4Address::Create("0.0.0.0", 8888);
    while(!es->bind(addr)){
        sleep(2);
    }
    es->start();
}
int main(int argc, char** argv){
    if(argc < 2){
        TW_LOG_INFO(s_logger) << "used as[" << argv[0] << " -t] or [" << argv[0] << " -b]";
        return 0;
    }
    if(!strcmp(argv[1], "-b")){
        type = 2;
    }
    tw::IOManager iom("server", 2, false);
    iom.schedule(run);
    return 0;
}