#pragma once
#include <memory>
#include <string>
#include <sstream>
#include <stdarg.h>
#include <vector>
namespace tw{
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
    static Level StringToLevel(const std::string& str);
};
class Logger;
class LogEvent{
public:
    using ptr = std::shared_ptr<LogEvent>;
    LogEvent(const char* file, uint32_t line, uint32_t elapse
            ,uint32_t threadID, uint32_t fiberID, uint64_t time
            ,std::string threadName, LogLevel::Level level, Logger::ptr logger);
    const char* getFile() const {return m_file;}
    uint32_t getLine() const {return m_line;}
    uint32_t getElapse() const {return m_elapse;}
    uint32_t getThreadID() const {return m_threadID;}
    uint32_t getFiberID() const {return m_fiberID;}
    uint64_t getTime() const {return m_time;};
    std::string getThreadName() const {return m_threadName;}
    LogLevel::Level getLevel() const {return m_level;}
    const std::string& getContent() const {return m_ss.str();}
    std::stringstream& getSS() {return m_ss;}

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
     Logger::ptr m_logger;
};

class LogFormatter{
public:
    using ptr = std::shared_ptr<LogFormatter>;
    LogFormatter(const std::string& pattern):m_pattern(pattern){
        init();
    }
    void init();
    std::string format(Logger::ptr logger, LogEvent::ptr event);
    bool isError()const {return m_error;}
public:
class FormatterItem{
public:
    using ptr = std::shared_ptr<FormatterItem>;
    virtual ~FormatterItem(){}
    virtual void format(std::ostream& os, Logger::ptr logger, LogEvent::ptr event) = 0;
};
private:
    const std::string& m_pattern;
    std::vector<FormatterItem::ptr> m_items;
    bool m_error = false;
};
class LogAppender{
public:
    using ptr = std::shared_ptr<LogAppender>;
    bool hasFormat()const { return has_format;}
    void setLogFormatter(LogFormatter::ptr formatter){
        m_formatter = formatter;
    }
    virtual ~LogAppender(){}
    virtual void log(Logger::ptr logger, LogEvent::ptr event) = 0;
protected:
    bool has_format = false;
    LogFormatter::ptr m_formatter;
};

class StdoutLogAppender : public LogAppender{
public:
    void log(Logger::ptr logger, LogEvent::ptr event);
};
class Logger : public std::enable_shared_from_this<Logger>{
public:
    using ptr = std::shared_ptr<Logger>;
    const char* getName() const {return m_name;}
private:
    LogLevel::Level m_level;
    const char* m_name;

};
}