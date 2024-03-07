#pragma once

#include "Base.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace GEngine
{
	
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

// Core Logging Macros
#define GE_CORE_TRACE(...)	Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GE_CORE_INFO(...)	Hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GE_CORE_WARN(...)	Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GE_CORE_ERROR(...)	Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GE_CORE_FATAL(...)	Hazel::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Logging Macros
#define GE_TRACE(...)	Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GE_INFO(...)	Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define GE_WARN(...)	Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GE_ERROR(...)	Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define GE_FATAL(...)	Hazel::Log::GetClientLogger()->critical(__VA_ARGS__)
}



