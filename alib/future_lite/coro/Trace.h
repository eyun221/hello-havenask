#ifndef FUTURE_LITE_CORO_TRACE_H
#define FUTURE_LITE_CORO_TRACE_H

#include <future_lite/Common.h>
#include <future_lite/Invoke.h>
#include <future_lite/Executor.h>
#include <future_lite/coro/Lazy.h>
#include <future_lite/coro/Event.h>
#include <alog/Logger.h>
#include <alog/Configurator.h>
#include <exception>

namespace future_lite {
namespace coro {

struct Tracer {
    FL_LOG_DECLARE();
};

}  // namespace coro
}  // namespace future_lite

#define FL_TRACE(executor, format, args...)                                                 \
    if (future_lite::coro::Tracer::_logger->isLevelEnabled(alog::LOG_LEVEL_TRACE1)) {       \
        size_t _fl_coro_trace_contextId = executor ? executor->currentContextId() : 0;      \
        future_lite::coro::Tracer::_logger->log(alog::LOG_LEVEL_TRACE1, __FILE__, __LINE__, \
                                                __FUNCTION__, "[CoroTrace][%lu] " format,   \
                                                _fl_coro_trace_contextId, ##args);          \
    }

#define FL_CORO_TRACE(format, args...)                                                      \
    if (future_lite::coro::Tracer::_logger->isLevelEnabled(alog::LOG_LEVEL_TRACE1)) {       \
        auto _fl_coro_trace_executor = co_await future_lite::CurrentExecutor();             \
        size_t _fl_coro_trace_contextId =                                                   \
            _fl_coro_trace_executor ? _fl_coro_trace_executor->currentContextId() : 0;      \
        future_lite::coro::Tracer::_logger->log(alog::LOG_LEVEL_TRACE1, __FILE__, __LINE__, \
                                                __FUNCTION__, "[CoroTrace][%lu] " format,   \
                                                _fl_coro_trace_contextId, ##args);          \
    }



#endif
