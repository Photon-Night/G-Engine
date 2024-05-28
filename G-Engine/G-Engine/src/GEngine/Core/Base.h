#pragma once

#include <memory>

namespace GEngine {

	void InitializeCore();
	void ShutdownCore();

}

#ifndef GE_PLATFORM_WINDOWS
	#error GEngine only supports Windows!
#endif

#define GE_EXPAND_VARGS(x) x

#define BIT(x) (1 << x)

#define GE_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)

#include "Assert.h"

namespace GEngine {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	using byte = uint8_t;

}