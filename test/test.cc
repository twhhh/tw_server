#include <iostream>
#include <string>
#include <unistd.h>
#include "tw/log.h"
#include "tw/util.h"
#include "tw/macro.h"
int main(){
    tw::Logger::ptr m_logger= TW_LOG_NAME("xiaoming");
    //m_logger->setLevel(tw::LogLevel::INFO);
    TW_LOG_DEBUG(m_logger) << "after setFomatter";
    TW_LOG_ERROR(m_logger) << "error";
    tw::LogAppender::ptr fileout = std::make_shared<tw::FileLogAppender>();
    m_logger->addAppender(fileout);
    TW_LOG_DEBUG(m_logger) << "after file";
    usleep(100000);
    TW_LOG_ERROR(m_logger) << "after file error";
    TW_ASSERT2(0 == 10, "test");
    return 0;
}