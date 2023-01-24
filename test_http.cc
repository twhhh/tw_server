#include "tw/http/http.h"
#include "tw/log.h"

void test_request(){
    tw::http::HttpRequest::ptr req(new tw::http::HttpRequest);
    req->setHeader("name", "tw");
    req->setBody("this is a body");
    req->dump(std::cout) << std::endl;
}

void test_respond(){
    tw::http::HttpRespond::ptr res(new tw::http::HttpRespond);
    res->setHeader("name", "tw");
    res->setBody("hello, world!");
    res->setStatus((tw::http::HttpStatus)400);
    res->setClose(false);

    res->dump(std::cout) << std::endl;
}
int main(){
    test_request();
    test_respond();
    return 0;
}