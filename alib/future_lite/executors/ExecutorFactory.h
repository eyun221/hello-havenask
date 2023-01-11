#ifndef FUTURE_LITE_EXECUTOR_FACTORY_H
#define FUTURE_LITE_EXECUTOR_FACTORY_H

#include <functional>
#include <thread>
#include <string>
#include "future_lite/Executor.h"

// namespace future_lite {

// class ExecutorFactory {
// public:
//     static Executor* create(const std::string& name, size_t threadNum);
// };

// }

extern "C" future_lite::Executor* createFutureLiteExecutor(const std::string& name, size_t threadNum);

#endif
