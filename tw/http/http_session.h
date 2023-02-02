#pragma once
#include <memory>
#include "../socket_stream.h"
#include "http.h"
namespace tw{
namespace http{
class HttpSession : public SocketStream{
public:
    using ptr = std::shared_ptr<HttpSession>;
    HttpSession(Socket::ptr sock, bool owner = true);
    HttpRequest::ptr recvRequest();
    int sendRespond(HttpRespond::ptr res);
};
}
}