#include "gepch.h"
#include "Log.h"

namespace GEngine {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("GEngine");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);

		GE_CORE_TRACE("Trace");
		GE_CORE_INFO("Info");
		GE_CORE_WARN("Warn");
		GE_CORE_ERROR("Error");
		GE_CORE_FATAL("Fatal");
	}

}