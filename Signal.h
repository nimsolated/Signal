#pragma once
#include <functional>
#include <iostream>
#include <vector>

template <typename RetVal, typename... Args>
class Signal {
public:
    using FuncPtr = std::function<RetVal(Args...)>;
public:
    Signal() = default;
    Signal(const std::vector<FuncPtr>& functions) : m_slots(functions) {}
    ~Signal() = default;

    // Lambdas, free functions, functors
    template <typename F>
    void connect(F&& func) {
        m_slots.emplace_back(std::forward<F>(func));
    }

    // Member function
    template <typename T>
    void connect(T* objectPtr, RetVal (T::*memberFunc)(Args...)) {
        m_slots.emplace_back([objectPtr, memberFunc](Args... args) {
            (objectPtr->*memberFunc)(std::forward<Args>(args)...);
        });
    }

    // invoke connected functions
    template <typename... CallArgs>
    auto emit(CallArgs&&... args) {
        static_assert(sizeof...(CallArgs) == sizeof...(Args), "Wrong number of arguments to emit()");

        // handle void vs non-void return values
        if constexpr (std::is_void_v<RetVal>) {
            for (auto& func : m_slots) {
                func(std::forward<CallArgs>(args)...);
            }
        } else {
            std::vector<RetVal> results;
            results.reserve(m_slots.size());

            for (auto& func : m_slots) {
                results.emplace_back(func(std::forward<CallArgs>(args)...));
            }
            return results;
        }
    }

private:
    // stores all connected functions
    std::vector<FuncPtr> m_slots;
};
