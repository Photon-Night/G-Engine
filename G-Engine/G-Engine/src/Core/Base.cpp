#include "gepch.h"
#include "Base.h"

#include "Log.h"

#define GEngine_BUILD_ID "v0.1a"

namespace GEngine {

	void InitializeCore()
	{
		GEngine::Log::Init();

		GE_CORE_TRACE("G-Engine {}", GEngine_BUILD_ID);
		GE_CORE_TRACE("Initializing...");
	}

	void ShutdownCore()
	{
		GE_CORE_TRACE("Shutting down...");
	}

}