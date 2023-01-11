#ifndef __BEEPER_LOG_H
#define __BEEPER_LOG_H

#include <alog/Logger.h>
#include <alog/Configurator.h>
#include <iostream>

#define BP_LOGGER_NAMESPACE alog

#define BP_ROOT_LOG_CONFIG() alog::Configurator::configureRootLogger()

#define BP_LOG_CONFIG(filename) do {                            \
        try {                                                   \
            alog::Configurator::configureLogger(filename);      \
        } catch(std::exception &e) {                            \
            std::cerr << "WARN! Failed to configure logger!"    \
                      << e.what() << ",use default log conf."   \
                      << std::endl;                             \
            BP_ROOT_LOG_CONFIG();                               \
        }                                                       \
    }while(0)

#define BP_ROOT_LOG_SETLEVEL(level)                                     \
    alog::Logger::getRootLogger()->setLevel(alog::LOG_LEVEL_##level)
#define BP_LOG_DECLARE() static alog::Logger *_logger
#define BP_LOG_SETUP(n,c) alog::Logger *c::_logger      \
    = alog::Logger::getLogger("beeper." #n "."  #c)
#define BP_LOG_TEST()                                           \
    do {                                                        \
        std::cout << "*** in log test of alog ***" << endl;     \
    }while(0)

#define BP_LOG_SETUP_TEMPLATE(n,c) template <typename T>        \
    alog::Logger *c<T>::_logger                                 \
    = alog::Logger::getLogger("beeper." #n "."  #c)

#define BP_LOG_SETUP_TEMPLATE2(n,c) template <typename T1, typename T2> \
    alog::Logger *c<T1, T2>::_logger                                    \
    = alog::Logger::getLogger("beeper." #n "."  #c)

#define BP_LOG_SETUP_TEMPLATE3(n,c) template <typename T1, typename T2, typename T3> \
    alog::Logger *c<T1, T2, T3>::_logger                                \
    = alog::Logger::getLogger("beeper." #n "."  #c)

#define BP_LOG_SETUP_TEMPLATE_CUSTOM(TT1, n, c)         \
    template <TT1 T1>                                   \
    alog::Logger *c<T1>::_logger                        \
    = alog::Logger::getLogger("beeper." #n "."  #c)

#define BP_LOG_SETUP_TEMPLATE2_CUSTOM(TT1, TT2, n, c)   \
    template <TT1 T1, TT2 T2>                           \
    alog::Logger *c<T1, T2>::_logger                    \
    = alog::Logger::getLogger("beeper." #n "."  #c)

#define BP_LOG_SETUP_TEMPLATE3_CUSTOM(TT1, TT2, TT3, n, c)      \
    template <TT1 T1, TT2 T2, TT3 T3>                           \
    alog::Logger *c<T1, T2, T3>::_logger                        \
    = alog::Logger::getLogger("beeper." #n "."  #c)

#define BP_LOG_SETUP_TEMPLATE4_CUSTOM(TT1, TT2, TT3, TT4, n, c) \
    template <TT1 T1, TT2 T2, TT3 T3, TT4 T4>                   \
    alog::Logger *c<T1, T2, T3, T4>::_logger                    \
    = alog::Logger::getLogger("beeper." #n "."  #c)

#define BP_LOG_SETUP_TEMPLATE5_CUSTOM(TT1, TT2, TT3, TT4, TT5, n, c)    \
    template <TT1 T1, TT2 T2, TT3 T3, TT4 T4, TT5 T5>                   \
    alog::Logger *c<T1, T2, T3, T4, T5>::_logger                        \
    = alog::Logger::getLogger("beeper." #n "."  #c)

#define BP_LOG(level, format, args...) ALOG_LOG(_logger, alog::LOG_LEVEL_##level, format, ##args)

#define BP_PREFIX_LOG(level, format, args...) BP_LOG(level, "[%s] " format, BP_LOG_PREFIX, ##args)

#define __INTERNAL_BP_LOG(level, format, args...) ALOG_LOG(_logger, level, format, ##args)


#define BP_INTERVAL_LOG(logInterval, level, format, args...)    \
    do {                                                        \
        static int logCounter;                                  \
        if (0 == logCounter) {                                  \
            BP_LOG(level, format, ##args);                      \
            logCounter = logInterval;                           \
        }                                                       \
        logCounter--;                                           \
    } while (0)

#define BP_INTERVAL_LOG2(logInterval, level, format, args...)   \
    do {                                                        \
        static int64_t logTimestamp;                            \
        int64_t now = TimeUtility::currentTimeInSeconds();      \
        if (now - logTimestamp > logInterval) {                 \
            BP_LOG(level, format, ##args);                      \
            logTimestamp = now;                                 \
        }                                                       \
    } while (0)


#define BP_DECLARE_AND_SETUP_LOGGER(n) static alog::Logger *_logger     \
    = alog::Logger::getLogger("beeper." #n)
#define BP_LOG_SHUTDOWN() alog::Logger::shutdown()
#define BP_LOG_FLUSH() alog::Logger::flushAll()

#endif
