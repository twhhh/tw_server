#include "http_server.h"
#include "http.h"
#include "../log.h"
namespace tw{
namespace http{
static tw::Logger::ptr m_logger = TW_LOG_NAME("system");
HttpServer::HttpServer(bool keepalive, tw::IOManager* worker, tw::IOManager* accept_worker)
    :TcpServer(worker, accept_worker)
    ,m_isKeepalive(keepalive){}
void HttpServer::handleClient(Socket::ptr client){
    HttpSession::ptr session(new HttpSession(client));
    do{
        auto req = session->recvRequest();
        if(!req){
            TW_LOG_WARN(m_logger) << "recv http fail, errno="
                << errno << " errstr=" << strerror(errno)
                << " client:" << *client;
            break;
        }
        HttpRespond::ptr res(new HttpRespond(req->getVersion(), req->isClose() || !m_isKeepalive));
        res->setBody("hello tw");

        TW_LOG_INFO(m_logger) << "request: " << std::endl
            << *req;
        TW_LOG_INFO(m_logger) << "respond: " << std::endl
            << *res;
        session->sendRespond(res);
    } while(m_isKeepalive);
    session->close();
}
}
}