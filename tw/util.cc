#include "util.h"
#include "log.h"
#include <execinfo.h>
#include <vector>
namespace tw{
static Logger::ptr m_logger = TW_LOG_NAME("system");
uint32_t GetFiberID(){
    return 0;
}
pid_t GetThreadID(){
    return syscall(SYS_gettid);
}

std::string BackTraceToString(){
    void** buffer = (void**)malloc(sizeof(void*) * 100);
    size_t inf = ::backtrace(buffer, 100);
    char** ans = ::backtrace_symbols(buffer, inf);
    if(ans == NULL){
        TW_LOG_ERROR(m_logger) << "backtrace error";
        return "error";
    }
    std::vector<std::string> rt;
    for(size_t i = 0; i < inf; ++i){
        rt.push_back(ans[i]);
    }
    free(buffer);
    free(ans);
    std::stringstream ss;
    for(size_t i = 0; i < (size_t)rt.size(); ++i){
        ss << rt[i] << std::endl;
    }
    return ss.str();
}
}