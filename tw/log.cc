#include <iostream>
#include <unordered_map>
#include <functional>
#include "log.h"
namespace tw{
LogEvent::LogEvent(const char* file, uint32_t line, uint32_t elapse
            ,uint32_t threadID, uint32_t fiberID, uint64_t time
            ,std::string threadName, LogLevel::Level level, Logger::ptr logger)
            :m_file(file)
            ,m_line(line)
            ,m_elapse(elapse)
            ,m_threadID(threadID)
            ,m_fiberID(fiberID)
            ,m_time(time)
            ,m_threadName(threadName)
            ,m_level(level)
            ,m_logger(logger){}
void LogEvent::format(const char* fmt, ...){
    va_list va;
    va_start(va, fmt);
    format(fmt, va);
    va_end(va);
}
void LogEvent::format(const char* fmt, va_list va){
    char* buf = nullptr;
    int len = vasprintf(&buf, fmt, va);
    if(len != -1){
        m_ss << std::string(buf);
        free(buf);
    }
}

class MsgFormatterItem : public LogFormatter::FormatterItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event)override{
        os << event->getContent();
    }
    
};
class FileFormatterItem : public LogFormatter::FormatterItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event)override{
        os << event->getFile();
    }
};
class LineFormatterItem : public LogFormatter::FormatterItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event)override{
        os << event->getLine();
    }   
};
class ElapseFormatterItem : public LogFormatter::FormatterItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event)override{
        os << event->getElapse();
    }
};
class ThreadIDFormatterItem : public LogFormatter::FormatterItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event)override{
        os << event->getThreadID();
    }
};
class FiberIDFormatterItem : public LogFormatter::FormatterItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event)override{
        os << event->getFiberID();
    }
};
class TimeFormatterItem : public LogFormatter::FormatterItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event)override{
        struct tm tm;
        std::string fo = "%Y-%m-%d %H:%M:%S";
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), fo.c_str(), &tm);
        os << buf;
    }
};
class ThreadNameFormatterItem : public LogFormatter::FormatterItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event)override{
        os << "\"" << event->getThreadName() << "\"";
    }
};
class LevelFormatterItem : public LogFormatter::FormatterItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event)override{
        os << "[ "<< LogLevel::LevelToString(event->getLevel()) << " ]";
    }
};
class TabFormatterItem : public LogFormatter::FormatterItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event)override{
        os << "\t";
    }
};
class EnterFormatterItem : public LogFormatter::FormatterItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event)override{
        os << std::endl;
    }
};
class NameFormatterItem : public LogFormatter::FormatterItem{
public:
    void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event)override{
        os << logger->getName();
    }
};
void LogFormatter::init(){
    if(m_pattern.empty()){
        return;
    }
    static std::unordered_map<std::string, std::function<FormatterItem::ptr()> >s_items_map{
#define XX(str, C)\
    {#str, [](){return FormatterItem::ptr(new C());}}
    XX(m, MsgFormatterItem),               //m:消息
    XX(p, LevelFormatterItem),             //p:日志级别
    XX(r, ElapseFormatterItem),            //r:累计毫秒数
    XX(c, NameFormatterItem),              //c:日志名称
    XX(t, ThreadIDFormatterItem),          //t:线程id
    XX(n, EnterFormatterItem),             //n:换行
    XX(d, TimeFormatterItem),              //d:时间
    XX(f, FileFormatterItem),              //f:文件名
    XX(l, LineFormatterItem),              //l:行号
    XX(T, TabFormatterItem),               //T:Tab
    XX(F, FiberIDFormatterItem),           //F:协程id
    XX(N, ThreadNameFormatterItem),        //N:线程名称
#undef XX
    };
    std::vector<std::string> vec;
    for(int i = 0; i < m_pattern.size(); ++i){
        if(m_pattern[i] == '%'){
            continue;
        }
        std::string str;
        str.append(1, m_pattern[i]);
        vec.push_back(str);
    }
    for(auto& i : vec){
        auto it = s_items_map.find(i);
        if(it == s_items_map.end()){
            m_error = true;
        }else{
            m_items.push_back(it->second());
        }
    }
}
std::string LogFormatter::format(Logger::ptr logger, LogEvent::ptr event){
    std::stringstream ss;
    if(!m_error){
        for(auto& i : m_items){
            i->format(ss, logger, event);
        }
    }
    return ss.str();
}

void StdoutLogAppender::log(Logger::ptr logger, LogEvent::ptr event){
    std::cout << m_formatter->format(logger, event);
}
}