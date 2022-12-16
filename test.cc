#include <iostream>
#include <string>
#include <unistd.h>
#include "tw/log.h"
#include "tw/util.h"
#include "tw/macro.h"
int main(){
    tw::Logger::ptr m_logger= TW_LOG_NAME("xiaoming");
    m_logger->setFormatter(tw::LogFormatter::ptr(new tw::LogFormatter("%p%c%T%m%n")));
    TW_LOG_DEBUG(m_logger) << "after setFomatter";
    m_logger->setFormatterDefault();
    TW_LOG_DEBUG(m_logger) << "after default";
    int a = 10;
    TW_LOG_FMT_DEBUG(m_logger, " ###%d###", a);
    TW_LOG_ERROR(m_logger) << "error";
    TW_ASSERT2(0 == 10, "test");
    return 0;
}