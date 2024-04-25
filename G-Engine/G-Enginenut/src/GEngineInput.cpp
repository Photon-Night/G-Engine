#include <GEngine.h>
#include <EntryPoint.h>
#include "TestLayer.h"

class GEngineApplicationTest : public GEngine::Application
{
public:
	GEngineApplicationTest(const GEngine::ApplicationProps& props)
		: Application(props)
	{
	}

	virtual void OnInit() override
	{
		PushLayer(new GEngine::TestLayer());
	}
};

GEngine::Application* GEngine::CreateApplication()
{
	return new GEngineApplicationTest({ "GEngine_Test", 1600, 900 });
}