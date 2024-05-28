#pragma once

#include "GEngine/Core/Base.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace GEngine {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

#define GE_CORE_TRACE(...)	GEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GE_CORE_INFO(...)	GEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GE_CORE_WARN(...)	GEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GE_CORE_ERROR(...)	GEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GE_CORE_FATAL(...)	GEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define GE_TRACE(...)	GEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GE_INFO(...)	GEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define GE_WARN(...)	GEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GE_ERROR(...)	GEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define GE_FATAL(...)	GEngine::Log::GetClientLogger()->critical(__VA_ARGS__)