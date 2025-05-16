#pragma once
#include <iostream>
#include <functional>
#include <vector>

template <typename RetVal, typename... Args>
class Signal {
public:
	// alias for function pointers (uses std::function from the functional library now)
	using FuncPtr = std::function<RetVal(Args...)>;
public:
	Signal() = default;
	Signal(const std::vector<FuncPtr>& functions) : m_functions(functions) {}
	~Signal() = default;

	// connecting a free function
	template <typename F>
	void connect(F&& func) {
		m_functions.emplace_back(std::forward<F>(func));
	}

	// connecting a capture-lambda or class member overload
	template <typename T>
	void connect(T* objectPtr, RetVal(T::*memberFunc)(Args...)) {
		m_functions.emplace_back(
			[objectPtr, memberFunc](Args... args) {
				(objectPtr->*memberFunc)(std::forward<Args>(args)...);
			}
		);
	}

	// emit all connected functions
	template <typename... CallArgs>
	RetVal emit(CallArgs&&... args) {
		static_assert(sizeof...(CallArgs) == sizeof...(Args), "Wrong number of arguments to emit()");

		for (auto& func : m_functions) {
			func(std::forward<Args>(args)...);
		}
	}
private:
	// stores all the connected functions
	std::vector<FuncPtr> m_functions;
};