#pragma once

#include <memory>

#ifdef _WIN32
	#define IDRA_WINDOW_GLFW
	#ifdef IDRA_BUILD_DLL
		#ifdef IDRA_CORE_EXPORT
			#define IDRA_API __declspec(dllexport)
		#else
			#define IDRA_API __declspec(dllimport)
		#endif
	#else
		#define IDRA_API // else ignore Macro, as we are building a static library
	#endif
#else
	#error Only Windows is supported!
#endif

#ifdef IDRA_DEBUG_MODE
	#define IDRA_ENABLE_ASSERTS
#endif 

#ifdef IDRA_ENABLE_ASSERTS
	#define IDRA_ASSERT(x, ...) { if(!(x)) { IDRA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define IDRA_CORE_ASSERT(x, ...) { if(!(x)) { IDRA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define IDRA_ASSERT(x, ...)
	#define IDRA_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define IDRA_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define IDRA_BIND_EVENT_WITH_ARGS(fn, ...) [&](auto& e) { return fn(e, __VA_ARGS__); }

namespace Idra {
	template<typename T> 
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}