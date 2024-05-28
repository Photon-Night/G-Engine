#pragma once
#include "GEngine.h"
#include "GEngine/ImGui/ImGuiLayer.h"
#include "imgui/imgui_internal.h"
#include "GEngine/Scene/Scene.h"
#include "GEngine/Scene/Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>
namespace GEngine
{
	class EditorLayer : public Layer
	{
	public:
		enum class PropertyFlag
		{
			None = 0, ColorProperty = 1
		};
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;

		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	private:
		std::pair<float, float> GetMouseViewportSpace();
		std::pair<glm::vec3, glm::vec3> CastRay(float mx, float my);

	private:

		Ref<Scene> m_Scene;
		Ref<Scene> m_ActiveScene;

		Entity* m_MeshEntity = nullptr;

		glm::vec2 m_ViewportBounds[2];
		float m_SnapValue = 0.5f;
		int m_GizmoType = -1;
		struct SelectedSubmesh
		{
			Submesh* Mesh;
			float Distance;
		};
		std::vector<SelectedSubmesh> m_SelectedSubmeshes;
		glm::mat4* m_CurrentlySelectedTransform = nullptr;
	};
}


