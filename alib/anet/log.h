#ifndef ANET_LOG_H
#define ANET_LOG_H

#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

#include <anet/ilogger.h>
#include <anet/alogadapter.h>

//;#include <anet/config.h>
#ifndef HAVE_ALOG
#define HAVE_ALOG
#endif

#ifndef HAVE_ALOG
#define ANET_LOG(level, ...) \
    anet::Logger::logMessage(ANET_LOG_LEVEL_##level, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else /* do logging with alog */

#include <alog/Logger.h>

#if !defined ALOG_VERSION || ALOG_VERSION < 010101
#error "From this version ANET requires ALOG >= 1.1.1. Please check your ALOG version. "
#endif

#define ANET_LOG_LEVEL_FATAL 1
#define ANET_LOG_LEVEL_ERROR 2
#define ANET_LOG_LEVEL_WARN  3
#define ANET_LOG_LEVEL_INFO  4
#define ANET_LOG_LEVEL_DEBUG 5
#define ANET_LOG_LEVEL_SPAM  6

#define ANET_LOG(level, format, ...) {                                  \
        if (__builtin_expect(anet::logger->isLevelEnabled(ANET_LOG_LEVEL_##level), 0)) \
            ANET_ALOG_##level(anet::logger, format, ##__VA_ARGS__);      \
    }

#define ANET_ALOG_FATAL(logger, format, ...) logger->log(ANET_LOG_LEVEL_FATAL,__FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define ANET_ALOG_ERROR(logger, format, ...) logger->log(ANET_LOG_LEVEL_ERROR,__FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define ANET_ALOG_WARN(logger, format, ...) logger->log(ANET_LOG_LEVEL_WARN,__FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define ANET_ALOG_INFO(logger, format, ...) logger->log(ANET_LOG_LEVEL_INFO,__FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define ANET_ALOG_DEBUG(logger, format, ...) logger->log(ANET_LOG_LEVEL_DEBUG,__FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define ANET_ALOG_SPAM(logger, format, ...) logger->log(ANET_LOG_LEVEL_SPAM,__FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)

#endif /*HAVE_ALOG*/

namespace anet {

#ifdef HAVE_ALOG
extern ILogger * logger;
inline ILogger * setAnetLogger(ILogger *l) { 
    ILogger *oldLogger = logger;
    logger = l;
    return oldLogger;
}

inline ILogger * getAnetLogger() { return logger; }

/* The namespace is to simulate the old static member of Logger class.
 * Now we don't have Logger class any longer so a namespace is provided
 * to application as an adapter to the "logger" object. 
 * By doing this, we can avoid update any existing code, however, the 
 * recompiling is still needed. */
namespace Logger{
    inline void logSetup() { ((AlogAdapter *)logger)->logSetup(); }
    inline void setLogLevel(const int level) { logger->setLogLevel(level); }
    inline void setLogLevel(const char * level) { logger->setLogLevel(level); }
}

#else
class Logger{
public:
    Logger();
    ~Logger();
    static void rotateLog(const char *filename);
    void logMessage(int level, const char *file, int line, const char *function, const char *fmt, ...);
    void setLogLevel(const char *level);
    void setLogLevel(const int level);
    static void logSetup();
    static void logSetup(const std::string &configFile);
    static void logTearDown();
};
#endif

}
#endif/*ANET_LOG_H*/
