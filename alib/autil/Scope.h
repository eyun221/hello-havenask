#ifndef AUTIL_SCOPE_H
#define AUTIL_SCOPE_H

#include <functional>

AUTIL_BEGIN_NAMESPACE(autil);

class ScopeGuard {
public:
    template <typename F>
    ScopeGuard(F&& f) : _exitFn(std::forward<F>(f)) {}

    ~ScopeGuard() {
        if (_exitFn) {
            _exitFn();
        }
    }

private:
    std::function<void()> _exitFn;
};

AUTIL_END_NAMESPACE(autil);

#endif
