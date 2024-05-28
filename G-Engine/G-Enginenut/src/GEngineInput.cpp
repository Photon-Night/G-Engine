#include <GEngine.h>
#include "GEngine/EntryPoint.h"
#include "EditorLayer.h"

class GEngineApplicationTest : public GEngine::Application
{
public:
	GEngineApplicationTest(const GEngine::ApplicationProps& props)
		: Application(props)
	{
	}

	virtual void OnInit() override
	{
		PushLayer(new GEngine::EditorLayer());
	}
};

GEngine::Application* GEngine::CreateApplication()
{
	return new GEngineApplicationTest({ "GEngine", 1600, 900 });
}