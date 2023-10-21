#include <iostream>
#include <unordered_map>
#include <functional>
#include <chrono>
#include "log.h"
namespace tw{
const char* LogLevel::LevelToString(Level level){
switch(level){
#define XX(name)\
    case(name): \
    return #name;\
    break;
    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
#undef XX
    default:
        return "UNKNOW";
}
}
LogLevel::Level LogLevel::StringToLevel(const std::string& str){
#define XX(level, v)\
    if(str == #v){\
        return LogLevel::level;\
    }
    XX(DEBUG, debug);
    XX(INFO, info);
    XX(WARN, warn);
    XX(ERROR, error);
    XX(FATAL, fatal);

    XX(DEBUG, DEBUG);
    XX(INFO, INFO);
    XX(WARN, WARN);
    XX(ERROR, ERROR);
    XX(FATAL, FATAL);
#undef XX
    return LogLevel::DEBUG;
}
LogEvent::LogEvent(const char* file, uint32_t line, uint32_t elapse
            ,uint32_t threadID, uint32_t fiberID, uint64_t time
            , LogLevel::Level level, std::shared_ptr<Logger> logger)
            :m_file(file)
            ,m_line(line)
            ,m_elapse(elapse)
            ,m_threadID(threadID)
            ,m_fiberID(fiberID)
            ,m_time(time)
            ,m_level(level)
            ,m_logger(logger){
    auto now = std::chrono::high_resolution_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    m_elapse = timestamp;
}

LogEventWrap::LogEventWrap(LogEvent::ptr event){
    m_event = event;
}
LogEventWrap::~LogEventWrap(){
    m_event->getLogger()->log(m_event);
}

void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogEvent::ptr event){
    struct tm tm;
    std::string fo = "%Y-%m-%d %H:%M:%S";
    time_t time = event->getTime();
    localtime_r(&time, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), fo.c_str(), &tm);
    std::lock_guard<std::mutex> lk(m_mutex);
    std::cout << "[" << std::string(buf) << " " << event->getElapse() << "]" << " " 
        << event->getThreadID() << " "
        << event->getFiberID() << " "
        << "["<< LogLevel::LevelToString(event->getLevel()) << "]"
        << logger->getName() << " "
        << event->getFile() << ":" << event->getLine() << " "
        << event->getContent() << std::endl;
}

FileLogAppender::FileLogAppender(){
    struct tm tm;
    std::string fo = "%Y-%m-%d-%H:%M:%S";
    time_t time1 = time(0);
    localtime_r(&time1, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), fo.c_str(), &tm);
    m_creatTime = std::string(buf);
    m_fs.open("tw_server_" + m_creatTime + ".log");
}

FileLogAppender::~FileLogAppender(){
    if(m_fs.is_open()){
        m_fs.close();
    }
}
void FileLogAppender::log(std::shared_ptr<Logger> logger, LogEvent::ptr event){
    struct tm tm;
    std::string fo = "%Y-%m-%d %H:%M:%S";
    time_t time = event->getTime();
    localtime_r(&time, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), fo.c_str(), &tm);
    std::lock_guard<std::mutex> lk(m_mutex);
    if(m_fs.is_open()){
        m_fs << "[" << std::string(buf) << " " << event->getElapse() << "]" << " " 
        << event->getThreadID() << " "
        << event->getFiberID() << " "
        << "["<< LogLevel::LevelToString(event->getLevel()) << "]"
        << logger->getName() << " "
        << event->getFile() << ":" << event->getLine() << " "
        << event->getContent() << std::endl;
    }
}


Logger::Logger(LogLevel::Level level, const std::string& name)
    :m_level(level)
    ,m_name(name){
    }
void Logger::addAppender(LogAppender::ptr appender){
    std::lock_guard<std::mutex> lk(m_mutex);
    for(auto& it : m_appenders){
        if(it == appender){
            return;
        }
    }
    m_appenders.push_back(appender);
}
void Logger::delAppender(LogAppender::ptr appender){
    std::lock_guard<std::mutex> lk(m_mutex);
    auto it = m_appenders.begin();
    for(; it != m_appenders.end(); ++it){
        if(*it == appender){
            m_appenders.erase(it);
            break;
        }
    }
}

void Logger::log(LogEvent::ptr event){
    if(m_appenders.empty()){
        return;
    }
    for(int i = 0; (size_t)i < m_appenders.size(); ++i){
        m_appenders[i]->log(shared_from_this(), event);
    }
}

LoggerManager::LoggerManager(){
    m_root.reset(new Logger(LogLevel::DEBUG, "root"));
    m_root->addAppender(LogAppender::ptr(new StdoutLogAppender()));
    m_map[m_root->getName()] = m_root;
}

Logger::ptr LoggerManager::getLogger(const std::string& name){
    std::lock_guard<std::mutex> lk(m_mutex);
    auto it = m_map.find(name);
    if(it != m_map.end()){
        return it->second;
    }
    Logger::ptr logger(new Logger(LogLevel::DEBUG, name));
    logger->addAppender(LogAppender::ptr(new StdoutLogAppender()));
    m_map[logger->getName()] = logger;
    return logger;
}
}