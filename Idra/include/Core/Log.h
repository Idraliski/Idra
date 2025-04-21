#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Idra {

	class IDRA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

} // namespace Idra

// core log macros
#ifdef IDRA_DEBUG_MODE
	#define IDRA_CORE_TRACE(...)    ::Idra::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define IDRA_CORE_INFO(...)     ::Idra::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define IDRA_CORE_WARN(...)     ::Idra::Log::GetCoreLogger()->warn(__VA_ARGS__)
#else
	#define IDRA_CORE_TRACE(...)    (void)0
	#define IDRA_CORE_INFO(...)     (void)0
	#define IDRA_CORE_WARN(...)     (void)0
#endif
#define IDRA_CORE_ERROR(...)    ::Idra::Log::GetCoreLogger()->error(__VA_ARGS__)
#define IDRA_CORE_CRITICAL(...) ::Idra::Log::GetCoreLogger()->critical(__VA_ARGS__)

// client log macros
#ifdef IDRA_DEBUG_MODE
	#define IDRA_TRACE(...)         ::Idra::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define IDRA_INFO(...)          ::Idra::Log::GetClientLogger()->info(__VA_ARGS__)
	#define IDRA_WARN(...)          ::Idra::Log::GetClientLogger()->warn(__VA_ARGS__)
#else
	#define IDRA_TRACE(...)         (void)0
	#define IDRA_INFO(...)          (void)0
	#define IDRA_WARN(...)          (void)0
#endif
#define IDRA_ERROR(...)         ::Idra::Log::GetClientLogger()->error(__VA_ARGS__)
#define IDRA_CRITICAL(...)      ::Idra::Log::GetClientLogger()->critical(__VA_ARGS__)