/*
 * Copyright 2014-2020 Alibaba Inc. All rights reserved.
 * Author Name: zongyuan.wuzy
 * Author Email: zongyuan.wuzy@alibaba-inc.com
 */

#ifndef KMONITOR_CLIENT_COMMON_LOG_H_
#define KMONITOR_CLIENT_COMMON_LOG_H_

#include <alog/Logger.h>
#include <alog/Configurator.h>
#include <iostream>
#include <string>
#include "kmonitor/client/common/Common.h"

BEGIN_KMONITOR_NAMESPACE(kmonitor);

#define KMONITOR_ALOG_CONF_FILE "kmonitor_alog.conf"

#define KMONITOR_LOG_SETUP(n, c)  alog::Logger *c::_logger \
    = alog::Logger::getLogger("kmonitor."#n "." #c)

#define KMONITOR_LOG_SETUP_TEMPLATE(n, c, T) template <typename T>   \
    alog::Logger *c<T>::_logger                                 \
    = alog::Logger::getLogger("kmonitor."#n "." #c)

#define KMONITOR_LOG_SETUP_TEMPLATE_2(n, c, T1, T2) template <typename T1, typename T2> \
    alog::Logger *c<T1, T2>::_logger                           \
    = alog::Logger::getLogger("kmonitor." #n "."  #c)

#define KMONITOR_LOG_SETUP_TEMPLATE_TEMPLATE(n, c, T, Template) template <template<typename T> class Template>   \
    alog::Logger *c<Template>::_logger                                 \
    = alog::Logger::getLogger("kmonitor."#n "." #c)

#define KMONITOR_ROOT_LOG_CONFIG() alog::Configurator::configureRootLogger()
#define KMONITOR_LOG_CONFIG(filename) do {                           \
    try {                                                   \
    alog::Configurator::configureLogger(filename);      \
    } catch(std::exception &e) {                            \
    std::cerr << "WARN! Failed to configure logger!"    \
    << e.what() << ",use default log conf."   \
    << std::endl;                             \
    KMONITOR_ROOT_LOG_CONFIG();                              \
    }                                                       \
    }while(0)

static const std::string kDefaultKmonAlogConfigString =
                   "alog.max_msg_len=4096\n"
                   "alog.rootLogger=INFO, KmonAppender\n"
                   "alog.appender.KmonAppender=ConsoleAppender\n"
                   "alog.appender.KmonAppender.layout=PatternLayout\n"
                   "alog.appender.KmonAppender.layout.LogPattern=[%%d] [%%l] [%%t,%%F -- %%f():%%n] [%%m]";

#define KMONITOR_LOG_CONFIG_FROM_STRING(content) do {                        \
    try {                                                           \
    alog::Configurator::configureLoggerFromString(content);     \
    } catch(std::exception &e) {                                    \
    std::cerr << "WARN! Failed to configure logger!"            \
    << e.what() << ",use default log conf."           \
    << std::endl;                                     \
    KMONITOR_ROOT_LOG_CONFIG();                                      \
    }                                                               \
    }while(0)

#define KMONITOR_ROOT_LOG_SETLEVEL(level)                                  \
    alog::Logger::getRootLogger()->setLevel(alog::LOG_LEVEL_##level)

#define KMONITOR_LOG_DECLARE() static alog::Logger *_logger

#define ENSURE_STRING_LITERAL(x)  do { (void) x ""; } while (0)

#ifndef KMONITOR_NDEBUG
#define KMONITOR_LOG(level, format, args...)                                 \
    do {                                                                \
    ENSURE_STRING_LITERAL(format);                                  \
    ALOG_LOG(_logger, alog::LOG_LEVEL_##level, format, ##args);     \
    } while (0)
#else
#define KMONITOR_LOG(level, format, args...) ALOG_LOG(_logger, alog::LOG_LEVEL_##level, format, ##args)
#endif

#define KMONITOR_DECLARE_AND_SETUP_LOGGER(c) static alog::Logger *_logger    \
    = alog::Logger::getLogger("kmonitor." #c)

#define KMONITOR_LOG_SHUTDOWN() alog::Logger::shutdown()

#define KMONITOR_LOG_FLUSH() alog::Logger::flushAll()

END_KMONITOR_NAMESPACE(kmonitor);

#endif  // KMONITOR_CLIENT_COMMON_LOG_H_
