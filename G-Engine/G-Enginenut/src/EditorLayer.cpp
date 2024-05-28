#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "GEngine/Renderer/SceneRenderer.h"
#include "GEngine/Core/Ray.h"
#include "GEngine/ImGui/ImGuizmo.h"

namespace GEngine
{
	EditorLayer::EditorLayer()
	{
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
		using namespace glm;
		auto environment = Environment::Load("assets/env/birchwood_4k.hdr");

		{
			m_Scene = CreateRef<Scene>("Model Scene");
			m_Scene->SetCamera(Camera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f)));
			m_Scene->SetEnvironment(environment);
			m_MeshEntity = m_Scene->CreateEntity("Test Entity");

			auto mesh = CreateRef<Mesh>("assets/meshes/TestScene.fbx");
			m_MeshEntity->SetMesh(mesh);
		}

		m_ActiveScene = m_Scene;

		auto& light = m_Scene->GetLight();
		light.Direction = { -0.5f, -0.5f, 1.0f };
		light.Radiance = { 2.0f, 2.0f, 2.0f };
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		m_Scene->GetCamera().OnUpdate(ts);
		m_ActiveScene->OnUpdate(ts);
		auto& light = m_Scene->GetLight();
		light.Direction = m_Scene->GetCamera().GetForwardDirection();
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool p_open = true;

		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		auto viewportOffset = ImGui::GetCursorPos();
		auto viewportSize = ImGui::GetContentRegionAvail();
		SceneRenderer::SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_ActiveScene->GetCamera().SetProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), viewportSize.x, viewportSize.y, 0.1f, 10000.0f));
		m_ActiveScene->GetCamera().SetViewportSize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		ImGui::Image((void*)SceneRenderer::GetFinalColorBufferRendererID(), viewportSize, { 0, 1 }, { 1, 0 });

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };

		ImGui::End();
		ImGui::PopStyleVar();

	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_Scene->GetCamera().OnEvent(e);
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case GE_KEY_Q:
			m_GizmoType = -1;
			break;
		case GE_KEY_W:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case GE_KEY_E:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case GE_KEY_R:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		auto [mx, my] = Input::GetMousePosition();
		if (e.GetMouseButton() == GE_MOUSE_BUTTON_LEFT && !Input::IsKeyPressed(GE_KEY_LEFT_ALT) && !ImGuizmo::IsOver())
		{
			auto [mouseX, mouseY] = GetMouseViewportSpace();
			if (mouseX > -1.0f && mouseX < 1.0f && mouseY > -1.0f && mouseY < 1.0f)
			{
				auto [origin, direction] = CastRay(mouseX, mouseY);

				m_SelectedSubmeshes.clear();
				auto mesh = m_MeshEntity->GetMesh();
				auto& submeshes = mesh->GetSubmeshes();
				float lastT = std::numeric_limits<float>::max();
				for (uint32_t i = 0; i < submeshes.size(); i++)
				{
					auto& submesh = submeshes[i];
					Ray ray = {
						glm::inverse(m_MeshEntity->GetTransform() * submesh.Transform) * glm::vec4(origin, 1.0f),
						glm::inverse(glm::mat3(m_MeshEntity->GetTransform()) * glm::mat3(submesh.Transform)) * direction
					};

					float t;
					bool intersects = ray.IntersectsBoundingBox(submesh.aabb, t);
					if (intersects)
					{
						const auto& triangleCache = mesh->GetTriangleCache(i);
						for (const auto& triangle : triangleCache)
						{
							if (ray.IntersectsTriangle(triangle.V0.Position, triangle.V1.Position, triangle.V2.Position, t))
							{
								GE_WARN("INTERSECTION: {0}, t={1}", submesh.NodeName, t);
								m_SelectedSubmeshes.push_back({ &submesh, t });
								break;
							}
						}
					}
				}
				std::sort(m_SelectedSubmeshes.begin(), m_SelectedSubmeshes.end(), [](auto& a, auto& b) { return a.Distance < b.Distance; });

				if (m_SelectedSubmeshes.size())
					m_CurrentlySelectedTransform = &m_SelectedSubmeshes[0].Mesh->Transform;
				else
					m_CurrentlySelectedTransform = &m_MeshEntity->Transform();
			}
		}
		return false;
	}

	std::pair<float, float> EditorLayer::GetMouseViewportSpace()
	{
		auto [mx, my] = ImGui::GetMousePos(); 
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		auto viewportWidth = m_ViewportBounds[1].x - m_ViewportBounds[0].x;
		auto viewportHeight = m_ViewportBounds[1].y - m_ViewportBounds[0].y;

		return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
	}

	std::pair<glm::vec3, glm::vec3> EditorLayer::CastRay(float mx, float my)
	{
		glm::vec4 mouseClipPos = { mx, my, -1.0f, 1.0f };

		auto inverseProj = glm::inverse(m_Scene->GetCamera().GetProjectionMatrix());
		auto inverseView = glm::inverse(glm::mat3(m_Scene->GetCamera().GetViewMatrix()));

		glm::vec4 ray = inverseProj * mouseClipPos;
		glm::vec3 rayPos = m_Scene->GetCamera().GetPosition();
		glm::vec3 rayDir = inverseView * glm::vec3(ray);

		return { rayPos, rayDir };
	}
}
