#pragma once
#include "http.h"
#include "http11_parser.h"
#include "httpclient_parser.h"

namespace tw{
namespace http{
class HttpRequestParser{
public:
    using ptr = std::shared_ptr<HttpRequestParser>;
    HttpRequestParser();
    size_t execute(char* data, size_t len);
    int isFinished();
    int hasError();
    HttpRequest::ptr getData() const { return m_data;}
    void setError(int v) {m_error = v;}
    uint64_t getContentLength();
    const http_parser& getParser() const {return m_parser;}
public:
    static uint64_t GetHttpRequestBufferSize();
    static uint64_t GetHttpRequestMaxBodySize();
private:
    http_parser m_parser;
    HttpRequest::ptr m_data;
    /// 错误码
    /// 1000: invalid method
    /// 1001: invalid version
    /// 1002: invalid field
    int m_error;
};

class HttpRespondParser{
public:
    using ptr = std::shared_ptr<HttpRespondParser>;
    HttpRespondParser();
    size_t execute(char* data, size_t len, bool chunck);
    int isFinished();
    int hasError();
    HttpRespond::ptr getData() const { return m_data;}
    void setError(int v) {m_error = v;}
    uint64_t getContentLength();
    const httpclient_parser& getParser() const {return m_parser;}
public:
    static uint64_t GetHttpRespondtBufferSize();
    static uint64_t GetHttpRespondMaxBodySize();
private:
    httpclient_parser m_parser;
    HttpRespond::ptr m_data;
     /// 错误码
    /// 1001: invalid version
    /// 1002: invalid field
    int m_error;
};
}
}