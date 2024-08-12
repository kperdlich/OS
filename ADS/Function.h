//
// Created by n3dry on 12.08.24.
//

#pragma once

#include "Assert.h"
#include "Move.h"
#include "OwnPtr.h"

namespace ADS {

template<typename>
class Function;

template<typename Ret, typename... Args>
class Function<Ret(Args...)> {
public:
    Function() = default;
    Function(ADS::nullptr_t) { }

    template<typename CallableType>
    Function(CallableType&& callable)
        : m_callableWrapper(makeOwn<CallableWrapper<CallableType>>(move(callable)))
    {
    }

    template<typename CallableType>
    Function& operator=(CallableType&& callable)
    {
        m_callableWrapper = makeOwn<CallableWrapper<CallableType>>(move(callable));
        return *this;
    }

    Function& operator=(std::nullptr_t)
    {
        m_callableWrapper = nullptr;
        return *this;
    }

    Ret operator()(Args... args) const
    {
        ASSERT(m_callableWrapper != nullptr);
        return m_callableWrapper->invoke(forward<Args>(args)...);
    }

    operator bool() const { return m_callableWrapper; }

private:
    class CallableBase {
    public:
        virtual ~CallableBase() = default;
        virtual Ret invoke(Args...) const = 0;
    };

    template<typename Callable>
    class CallableWrapper final : public CallableBase {
    public:
        explicit CallableWrapper(Callable&& callable)
            : m_callable(ADS::move(callable))
        {
        }

        CallableWrapper(const CallableWrapper&) = delete;
        CallableWrapper& operator=(const CallableWrapper&) = delete;

        Ret invoke(Args... args) const final override { return m_callable(ADS::forward<Args>(args)...); }

    private:
        Callable m_callable;
    };

    OwnPtr<CallableBase> m_callableWrapper;
};

}
