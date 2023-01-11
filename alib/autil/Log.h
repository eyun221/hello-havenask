#ifndef AUTIL_LOG_H
#define AUTIL_LOG_H

#include <alog/Logger.h>
#include <alog/Configurator.h>
#include <iostream>
#include "autil/TimeUtility.h"

#define AUTIL_ROOT_LOG_CONFIG() alog::Configurator::configureRootLogger()
#define AUTIL_LOG_CONFIG(filename) do {                         \
        try {                                                   \
            alog::Configurator::configureLogger(filename);      \
        } catch(std::exception &e) {                            \
            std::cerr << "WARN! Failed to configure logger!"    \
                      << e.what() << ",use default log conf."   \
                      << std::endl;                             \
            AUTIL_ROOT_LOG_CONFIG();                            \
        }                                                       \
    }while(0)

#define AUTIL_LOG_CONFIG_FROM_STRING(content) do {                  \
        try {                                                       \
            alog::Configurator::configureLoggerFromString(content); \
        } catch(std::exception &e) {                                \
            std::cerr << "configure logger use ["                   \
                      << content<< "] failed"                       \
                      << std::endl;                                 \
            AUTIL_ROOT_LOG_CONFIG();                                \
        }                                                           \
    }while(0)


#define AUTIL_ROOT_LOG_SETLEVEL(level)                                  \
    alog::Logger::getRootLogger()->setLevel(alog::LOG_LEVEL_##level)

#define AUTIL_LOG_DECLARE() static alog::Logger *_logger

#define AUTIL_LOG_SETUP(n, c) alog::Logger *c::_logger  \
    = alog::Logger::getLogger(#n "." #c)

#define AUTIL_LOG_SETUP_TEMPLATE(n, c, T) template <typename T> \
    alog::Logger *c<T>::_logger                                 \
    = alog::Logger::getLogger(#n "." #c)

#define AUTIL_LOG_SETUP_TEMPLATE_2(n, c, T1, T2) template <typename T1, typename T2> \
    alog::Logger *c<T1,T2>::_logger                                     \
    = alog::Logger::getLogger(#n "." #c)

#define AUTIL_LOG(level, format, args...) ALOG_LOG(_logger, alog::LOG_LEVEL_##level, format, ##args)

#define AUTIL_LOG_BINARY(level, msg) {                                  \
        if(__builtin_expect(_logger->isLevelEnabled(alog::LOG_LEVEL_##level), 0)) \
            _logger->logBinaryMessage(alog::LOG_LEVEL_##level, msg);    \
    }

#define AUTIL_DECLARE_AND_SETUP_LOGGER(n, c) static alog::Logger *_logger \
    = alog::Logger::getLogger(#n "." #c)
#define AUTIL_LOG_SHUTDOWN() alog::Logger::shutdown()
#define AUTIL_LOG_FLUSH() alog::Logger::flushAll()

#define AUTIL_INTERVAL_LOG(logInterval, level, format, args...)         \
    do {                                                                \
        static int logCounter;                                          \
        if (0 == logCounter) {                                          \
            AUTIL_LOG(level, format, ##args);                           \
            logCounter = logInterval;                                   \
        }                                                               \
        logCounter--;                                                   \
    } while (0)

#define AUTIL_INTERVAL_LOG2(logInterval, level, format, args...)        \
    do {                                                                \
        static int64_t logTimestamp;                                    \
        int64_t now = autil::TimeUtility::currentTimeInSeconds();       \
        if (now - logTimestamp > logInterval) {                         \
            AUTIL_LOG(level, format, ##args);                           \
            logTimestamp = now;                                         \
        }                                                               \
    } while (0)

#endif //AUTIL_LOG_H_
