#include "http_session.h"
#include "http_parser.h"
#include "../log.h"
namespace tw{
namespace http{
tw::Logger::ptr m_logger = TW_LOG_NAME("system");
HttpSession::HttpSession(Socket::ptr sock, bool owner)
    :SocketStream(sock, owner){
}
HttpRequest::ptr HttpSession::recvRequest(){
    HttpRequestParser::ptr parser(new HttpRequestParser);
    uint64_t buff_size = HttpRequestParser::GetHttpRequestBufferSize();
    std::shared_ptr<char> buffer(new char[buff_size], [](char* ptr){
        delete [] ptr;
    });
    char* data = buffer.get();
    int offset = 0;//解析字节数
    do{
        int len = read(data + offset, buff_size - offset);
        if(len <= 0){
            close();
            return nullptr;
        }
        len += offset;
        size_t nparser = parser->execute(data, len);
        if(parser->hasError()){
            close();
            TW_LOG_WARN(m_logger) << "parser->hasError";
            return nullptr;
        }
        offset = len - nparser;
        if(offset == (int)buff_size){
            close();
            return nullptr;
        }
        if(parser->isFinished()){
            break;
        }
    } while(true);//has something wrong
    int64_t length = parser->getContentLength();
    if(length > 0){
        std::string body;
        body.resize(length);
        int len = 0;
        if(length >= offset){
            memcpy(&body[0], data, offset);
            len = offset;
        } else {
            memcpy(&body[0], data, length);
            len = length;
        }
        length -= offset;
        if(length > 0){
            if(readFixSize(&body[len], length) <= 0){
                close();
                return nullptr;
            }
        }
        parser->getData()->setBody(body);
    }
    return parser->getData();
}
int HttpSession::sendRespond(HttpRespond::ptr res){
    std::stringstream ss;
    ss << *res;
    std::string data = ss.str();
    return writeFixSize(data.c_str(), data.size());
}
}
}