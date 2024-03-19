#include <GEngine.h>
#include <EntryPoint.h>

class GEngineApplication : public GEngine::Application
{
public:
	GEngineApplication(const GEngine::ApplicationProps& props)
		: Application(props)
	{
	}
};

GEngine::Application* GEngine::CreateApplication()
{
	return new GEngineApplication({ "GEngine", 1600, 900 });
}