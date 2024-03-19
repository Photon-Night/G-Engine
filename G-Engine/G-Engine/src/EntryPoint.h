#pragma once

#ifdef GE_PLATFORM_WINDOWS

extern GEngine::Application* GEngine::CreateApplication();

int main(int argc, char** argv)
{
	GEngine::InitializeCore();
	GEngine::Application* app = GEngine::CreateApplication();
	GE_CORE_ASSERT(app, "Client Application is null!");
	app->Run();
	delete app;
	GEngine::ShutdownCore();
}

#endif
