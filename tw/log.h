#pragma once
#include <memory>
#include <string>
#include <sstream>
#include <stdarg.h>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <fstream>
#include "singleton.h"
#include "util.h"

#define TW_LOG_LEVEL(logger, level)\
    if(logger->getLevel() <= level)\
        tw::LogEventWrap(tw::LogEvent::ptr(new tw::LogEvent(__FILE__, __LINE__,\
        0, tw::GetThreadID(), tw::GetFiberID(), time(0), level, logger))).getSS()

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
            , LogLevel::Level level, std::shared_ptr<Logger> logger);
    const char* getFile() const {return m_file;}
    uint32_t getLine() const {return m_line;}
    uint32_t getElapse() const {return m_elapse;}
    uint32_t getThreadID() const {return m_threadID;}
    uint32_t getFiberID() const {return m_fiberID;}
    uint64_t getTime() const {return m_time;};
    LogLevel::Level getLevel() const {return m_level;}
    std::string getContent() {return m_ss.str();}
    std::stringstream& getSS() {return m_ss;}
    std::shared_ptr<Logger> getLogger() {return m_logger;}

private:
    const char* m_file = nullptr;
    uint32_t m_line = 0;
    uint32_t m_elapse = 0;
    uint32_t m_threadID = 0;
    uint32_t m_fiberID = 0;
    uint64_t m_time = 0;
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
class LogAppender{
public:
    using ptr = std::shared_ptr<LogAppender>;
    virtual ~LogAppender(){}
    virtual void log(std::shared_ptr<Logger> logger, LogEvent::ptr event) = 0;
protected:
    std::mutex m_mutex;
};

class StdoutLogAppender : public LogAppender{
public:
    void log(std::shared_ptr<Logger> logger, LogEvent::ptr event) override;
};

class FileLogAppender : public LogAppender{
public:
    FileLogAppender();
    ~FileLogAppender();
    void log(std::shared_ptr<Logger> logger, LogEvent::ptr event) override;
private:
    std::string m_creatTime = "";
    std::ofstream m_fs;
};

class Logger : public std::enable_shared_from_this<Logger>{
friend class LoggerManager;
public:
    using ptr = std::shared_ptr<Logger>;
    Logger(LogLevel::Level, const std::string& name);
    std::string getName() const {return m_name;}
    LogLevel::Level getLevel() const {return m_level;}
    void setName(const char* name) {m_name = name;}
    void setLevel(LogLevel::Level level){m_level = level;}
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    void log(LogEvent::ptr event);
private:
    std::mutex m_mutex;
    LogLevel::Level m_level;
    std::string m_name;
    std::vector<LogAppender::ptr> m_appenders;
};

class LoggerManager{
public:
    LoggerManager();
    Logger::ptr getLogger(const std::string& name);
    Logger::ptr getRoot(){return m_root;}
private:
    std::mutex m_mutex;
    std::unordered_map<std::string, Logger::ptr> m_map;
    Logger::ptr m_root;
};

using LoggerMgr = tw::Singleton<LoggerManager>;
}