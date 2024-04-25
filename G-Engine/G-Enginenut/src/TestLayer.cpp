#include "TestLayer.h"

namespace GEngine
{
	TestLayer::TestLayer()
	{

	}

	TestLayer::~TestLayer()
	{

	}

	void TestLayer::OnAttach()
	{
		auto environment = Environment::Load("assets/env/birchwood_4k.hdr");
		m_Scene = CreateRef<Scene>("Model Scene");
		m_Scene->SetCamera(Camera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f)));

		m_Scene->SetEnvironment(environment);

		m_MeshEntity = m_Scene->CreateEntity("Test Entity");

		auto mesh = CreateRef<Mesh>("assets/meshes/TestScene.fbx");
		m_MeshEntity->SetMesh(mesh);

		m_MeshMaterial = mesh->GetMaterial();
		m_ActiveScene = m_Scene;

		m_PlaneMesh.reset(new Mesh("assets/models/Plane1m.obj"));
		auto& light = m_Scene->GetLight();
		light.Direction = { -0.5f, -0.5f, 1.0f };
		light.Radiance = { 1.0f, 1.0f, 1.0f };
	}

	void TestLayer::OnDetach()
	{

	}

	void TestLayer::OnUpdate(Timestep ts)
	{

	}

	void TestLayer::OnImGuiRender()
	{

	}
	void TestLayer::OnEvent(Event& e)
	{
	}
}
