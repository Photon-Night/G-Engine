#pragma once
#include "Core/Layer.h"
#include "Scene/Scene.h"

namespace GEngine
{
	class TestLayer : public Layer
	{
	public:
		TestLayer();
		virtual ~TestLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		Ref<Scene> m_Scene;
		Ref<Scene> m_ActiveScene;
		Entity* m_MeshEntity = nullptr;
		Ref<Material> m_MeshMaterial;
		Ref<Mesh> m_PlaneMesh;
	};
}


