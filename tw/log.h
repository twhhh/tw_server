#pragma once
#include <memory>
#include <string>
#include <sstream>
#include <stdarg.h>
#include <vector>
#include <unordered_map>

#include "singleton.h"
#include "util.h"
#include "thread.h"
#define TW_LOG_LEVEL(logger, level)\
    if(logger->getLevel() <= level)\
        tw::LogEventWrap(tw::LogEvent::ptr(new tw::LogEvent(__FILE__, __LINE__,\
        0, tw::GetThreadID(), tw::GetFiberID(), time(0), tw::Thread::GetName(), level, logger))).getSS()

#define TW_LOG_DEBUG(logger)\
    TW_LOG_LEVEL(logger, tw::LogLevel::DEBUG)

#define TW_LOG_WARN(logger)\
    TW_LOG_LEVEL(logger, tw::LogLevel::WARN)

#define TW_LOG_ERROR(logger)\
    TW_LOG_LEVEL(logger, tw::LogLevel::ERROR)

#define TW_LOG_FATAL(logger)\
    TW_LOG_LEVEL(logger, tw::LogLevel::FATAL)

#define TW_LOG_INFO(logger)\
    TW_LOG_LEVEL(logger, tw::LogLevel::INFO)

#define TW_LOG_FMT_LEVEL(logger, level, fmt, ...)\
    if(logger->getLevel() <= level)\
        tw::LogEventWrap(tw::LogEvent::ptr(new tw::LogEvent(__FILE__, __LINE__,\
        0, tw::GetThreadID(), tw::GetFiberID(), time(0), "thread name", level, logger))).getEvent()->format(fmt, ##__VA_ARGS__)

#define TW_LOG_FMT_DEBUG(logger, fmt, ...)\
    TW_LOG_FMT_LEVEL(logger, tw::LogLevel::DEBUG, fmt, ##__VA_ARGS__)

#define TW_LOG_FMT_WARN(logger, fmt, ...)\
    TW_LOG_FMT_LEVEL(logger, tw::LogLevel::WARN, fmt , ##__VA_ARGS__)

#define TW_LOG_FMT_ERROR(logger, fmt, ...)\
    TW_LOG_FMT_LEVEL(logger, tw::LogLevel::ERROR, fmt, ##__VA_ARGS__)

#define TW_LOG_FMT_FATAL(logger, fmt, ...)\
    TW_LOG_FMT_LEVEL(logger, tw::LogLevel::FATAL, fmt, ##__VA_ARGS__)

#define TW_LOG_FMT_INFO(logger, fmt, ...)\
    TW_LOG_FMT_LEVEL(logger, tw::LogLevel::INFO, fmt, ##__VA_ARGS__)

#define TW_LOG_ROOT() tw::LoggerMgr::GetInstance()->getRoot()
#define TW_LOG_NAME(name) tw::LoggerMgr::GetInstance()->getLogger(name)

namespace tw{
class Logger;
class LogLevel{
public:
    enum Level{
        DEBUG = 1,
        INFO,
        WARN,
        ERROR,
        FATAL
    };
    static const char* LevelToString(Level level);
    static LogLevel::Level StringToLevel(const std::string& str);
};
class LogEvent : public std::enable_shared_from_this<LogEvent>{
public:
    using ptr = std::shared_ptr<LogEvent>;
    LogEvent(const char* file, uint32_t line, uint32_t elapse
            ,uint32_t threadID, uint32_t fiberID, uint64_t time
            ,std::string threadName, LogLevel::Level level, std::shared_ptr<Logger> logger);
    const char* getFile() const {return m_file;}
    uint32_t getLine() const {return m_line;}
    uint32_t getElapse() const {return m_elapse;}
    uint32_t getThreadID() const {return m_threadID;}
    uint32_t getFiberID() const {return m_fiberID;}
    uint64_t getTime() const {return m_time;};
    std::string getThreadName() const {return m_threadName;}
    LogLevel::Level getLevel() const {return m_level;}
    std::string getContent() {return m_ss.str();}
    std::stringstream& getSS() {return m_ss;}
    std::shared_ptr<Logger> getLogger() {return m_logger;}

    void format(const char* fmt, ...);
    void format(const char* fmt, va_list va);
private:
    const char* m_file = nullptr;
    uint32_t m_line = 0;
    uint32_t m_elapse = 0;
    uint32_t m_threadID = 0;
    uint32_t m_fiberID = 0;
    uint64_t m_time = 0;
    std::string m_threadName;
    LogLevel::Level m_level;
    std::stringstream m_ss;
    std::shared_ptr<Logger> m_logger;
};

class LogEventWrap{
public:
    LogEventWrap(LogEvent::ptr event);
    std::stringstream& getSS(){return m_event->getSS();}
    LogEvent::ptr getEvent(){return m_event;}
    ~LogEventWrap();
private:
    LogEvent::ptr m_event;
};
class LogFormatter{
public:
    using ptr = std::shared_ptr<LogFormatter>;
    LogFormatter(const std::string& pattern):m_pattern(pattern){
        init();
    }
    void init();
    std::string format(std::shared_ptr<Logger> logger, LogEvent::ptr event);
    bool isError()const {return m_error;}
public:
class FormatterItem{
public:
    using ptr = std::shared_ptr<FormatterItem>;
    virtual ~FormatterItem(){}
    virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogEvent::ptr event) = 0;
};
private:
    const std::string& m_pattern;
    std::vector<FormatterItem::ptr> m_items;
    bool m_error = false;
};
class LogAppender{
public:
    using ptr = std::shared_ptr<LogAppender>;
    using MutexType = SpinMutex;
    bool hasFormat(){ 
        MutexType::Lock lock(m_mutex);
        return has_format;
    }
    void setLogFormatter(LogFormatter::ptr formatter){
        MutexType::Lock lock(m_mutex);
        has_format = true;
        m_formatter = formatter;
    }
    virtual ~LogAppender(){}
    virtual void log(std::shared_ptr<Logger> logger, LogEvent::ptr event) = 0;
protected:
    MutexType m_mutex;
    bool has_format = false;
    LogFormatter::ptr m_formatter;
};

class StdoutLogAppender : public LogAppender{
public:
    void log(std::shared_ptr<Logger> logger, LogEvent::ptr event);
};

class Logger : public std::enable_shared_from_this<Logger>{
friend class LoggerManager;
public:
    using ptr = std::shared_ptr<Logger>;
    using MutexType = SpinMutex;
    Logger(LogLevel::Level, const std::string& name);
    std::string getName() const {return m_name;}
    LogLevel::Level getLevel() const {return m_level;}
    void setName(const char* name) {m_name = name;}
    void setLevel(LogLevel::Level level){m_level = level;}
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    void setFormatter(LogFormatter::ptr formatter);
    void setFormatterDefault();
    void log(LogEvent::ptr event);
private:
    MutexType m_mutex;
    LogLevel::Level m_level;
    std::string m_name;
    std::vector<LogAppender::ptr> m_appenders;
};

class LoggerManager{
public:
    using MutexType = SpinMutex;
    LoggerManager();
    Logger::ptr getLogger(const std::string& name);
    Logger::ptr getRoot(){return m_root;}
private:
    MutexType m_mutex;
    std::unordered_map<std::string, Logger::ptr> m_map;
    Logger::ptr m_root;
};

using LoggerMgr = tw::Singleton<LoggerManager>;
}