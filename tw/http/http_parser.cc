#include "http_parser.h"
#include "../log.h"
namespace tw{
namespace http{
static tw::Logger::ptr m_logger = TW_LOG_NAME("system");
static uint64_t s_http_request_buffer_size = 4 * 1024;
static uint64_t s_http_request_max_body_size = 64 * 1024 * 1024;
static uint64_t s_http_respond_buffer_size = 4 * 1024;
static uint64_t s_http_respond_max_body_size = 64 * 1024 * 1024;

void on_request_http_field(void *data, const char *field, size_t flen, const char *value, size_t vlen){
    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    if(flen == 0){
        TW_LOG_WARN(m_logger) << "invalid http request field length = 0";
        parser->setError(1002);
        return;
    }
    parser->getData()->setHeader(std::string(field, flen)
    , std::string(value, vlen));
}
void on_request_method(void *data, const char *at, size_t length){
    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    HttpMethod m = CharsToHttpMethod(at);

    if(m == HttpMethod::INVALID_MATHOD){
        TW_LOG_WARN(m_logger) << "invalid http request mathod: "
            << std::string(at, length);
        parser->setError(1000);
        return;
    }
    parser->getData()->setMethod(m);
}
void on_request_uri(void *data, const char *at, size_t length){
   //HttpRequestParser* parser = static_cast<HttpRequestParser*> data;
}
void on_request_fragment(void *data, const char *at, size_t length){
    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    parser->getData()->setFragment(std::string(at, length));
}
void on_request_path(void *data, const char *at, size_t length){
    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    parser->getData()->setPath(std::string(at, length));
}
void on_request_query_string(void *data, const char *at, size_t length){
    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    parser->getData()->setQuery(std::string(at, length));
}

void on_request_http_version(void *data, const char *at, size_t length){
    HttpRequestParser* parser = static_cast<HttpRequestParser*>(data);
    uint8_t v = 0;
    if(strncmp(at, "HTTP/1.1", length) == 0){
        v = 0x11;
    } else if(strncmp(at, "HTTP/1.0", length) == 0){
        v = 0x10;
    } else {
        TW_LOG_WARN(m_logger) << "invalid http request version: "
            << std::string(at, length);
        parser->setError(1001);
        return;
    }
    parser->getData()->setVersion(v);
}
void on_request_header_done(void *data, const char *at, size_t length){

}
HttpRequestParser::HttpRequestParser()
    :m_error(0){
    m_data.reset(new HttpRequest);
    http_parser_init(&m_parser);
    m_parser.http_field = on_request_http_field;
    m_parser.request_method = on_request_method;
    m_parser.request_uri = on_request_uri;
    m_parser.fragment = on_request_fragment;
    m_parser.request_path = on_request_path;
    m_parser.query_string = on_request_query_string;
    m_parser.http_version = on_request_http_version;
    m_parser.header_done = on_request_header_done;
    m_parser.data = this;
}
size_t HttpRequestParser::execute(char* data, size_t len){
    size_t offset = http_parser_execute(&m_parser, data, len, 0);
    memmove(data, data + offset, (len - offset));
    return offset;
}
int HttpRequestParser::isFinished(){
    return http_parser_finish(&m_parser);
}
int HttpRequestParser::hasError(){
    return m_error || http_parser_has_error(&m_parser);
}
uint64_t HttpRequestParser::getContentLength(){
    return m_data->getHeaderAs<uint64_t>("content-length", 0);
}
uint64_t HttpRequestParser::GetHttpRequestBufferSize(){
    return s_http_request_buffer_size;
}
uint64_t HttpRequestParser::GetHttpRequestMaxBodySize(){
    return s_http_request_max_body_size;
}

void on_respond_http_field(void *data, const char *field, size_t flen, const char *value, size_t vlen){
    HttpRespondParser* parser = static_cast<HttpRespondParser*>(data);
    if(flen == 0){
        TW_LOG_WARN(m_logger) << "invalid http respond field length = 0";
        parser->setError(1002);
        return;
    }
    parser->getData()->setHeader(std::string(field, flen), std::string(value, vlen));
}
void on_respond_reason_phrase(void *data, const char *at, size_t length){
    HttpRespondParser* parser = static_cast<HttpRespondParser*>(data);
    parser->getData()->setReason(std::string(at, length));
}
void on_respond_status_code(void *data, const char *at, size_t length){
    HttpRespondParser* parser = static_cast<HttpRespondParser*>(data);
    HttpStatus status = (HttpStatus)(atoi(at));
    parser->getData()->setStatus(status);
}
void on_respond_chunk_size(void *data, const char *at, size_t length){

}
void on_respond_http_version(void *data, const char *at, size_t length){
    HttpRespondParser* parser = static_cast<HttpRespondParser*>(data);
    uint8_t v = 0;
    if(strncmp(at, "HTTP/1.1", length) == 0){
        v = 0x11;
    } else if(strncmp(at, "HTTP/1.0", length) == 0){
        v = 0x10;
    } else {
        TW_LOG_WARN(m_logger) << "invalid http respond version: "
            << std::string(at, length);
        parser->setError(1001);
        return;
    }
    parser->getData()->setVersion(v);
}
void on_respond_header_done(void *data, const char *at, size_t length){

}
void on_respond_last_chunk(void *data, const char *at, size_t length){

}

HttpRespondParser::HttpRespondParser()
    :m_error(0){
    m_data.reset(new HttpRespond());
    httpclient_parser_init(&m_parser);
    m_parser.http_field = on_respond_http_field;
    m_parser.reason_phrase = on_respond_reason_phrase;
    m_parser.status_code = on_respond_status_code;
    m_parser.chunk_size = on_respond_chunk_size;
    m_parser.http_version = on_respond_http_version;
    m_parser.header_done = on_respond_header_done;
    m_parser.last_chunk = on_respond_last_chunk;
    m_parser.data = this;
}

size_t HttpRespondParser::execute(char* data, size_t len, bool chunck){
    if(chunck){
        httpclient_parser_init(&m_parser);
    }
    size_t offset = httpclient_parser_execute(&m_parser, data, len, 0);
    memmove(data, data + offset, (len - offset));
    return offset;
}

int HttpRespondParser::isFinished(){
    return httpclient_parser_finish(&m_parser);
}
int HttpRespondParser::hasError(){
    return m_error || httpclient_parser_has_error(&m_parser);
}

uint64_t HttpRespondParser::getContentLength(){
    return m_data->getHeaderAs<uint64_t>("content-length", 0);
}

uint64_t HttpRespondParser::GetHttpRespondtBufferSize(){
    return s_http_respond_buffer_size;
}
uint64_t HttpRespondParser::GetHttpRespondMaxBodySize(){
    return s_http_respond_max_body_size;
}
}
}