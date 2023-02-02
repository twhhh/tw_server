#pragma once
#include "../tcp_server.h"
#include "http_session.h"

namespace tw{
namespace http{
class HttpServer : public TcpServer{
public:
    using ptr = std::shared_ptr<HttpServer>;
    HttpServer(bool keepalive = false
        , tw::IOManager* worker = tw::IOManager::GetThis()
        , tw::IOManager* accept_worker = 
        tw::IOManager::GetThis());
protected:
    virtual void handleClient(Socket::ptr client) override;
private:
    bool m_isKeepalive;
};
}
}