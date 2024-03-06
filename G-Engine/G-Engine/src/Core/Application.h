#pragma once
#include <string>
#include "Timestep.h"
#include "Event/Event.h"
#include "Base.h"
#include "Window.h"
#include "Layer.h"
#include "LayerStack.h"
#include "Event/ApplicationEvent.h"

namespace GEngine
{
	struct ApplicationProps
	{
		std::string name;
		uint32_t windowWidth, windowHeight;
	};

	class Application
	{
	public:
		Application(const ApplicationProps& props = {"G-Engine", 1280, 720});
		virtual ~Application();

		void Run();

		virtual void OnInit();
		virtual void OnShutdown();
		virtual void OnUpdate(Timestep delta) {}

		virtual void OnEvent(Event& event);
		virtual void PushLayer(Layer* layer);
		virtual void PushOverlay(Layer* layer);
		void RenderGUI();

		std::string OpenFile(const std::string& filter) const;
		inline Window& GetWindow() { return *m_Window; }
		static inline Application& GetInstance() {return *s_Instance;}

		float GetTime() const;

	private:
		bool OnWindowResize(WindowResizeEvent& event);
		bool OnWindowClose(WindowCloseEvent& event);

	private:
		Scope<Window> m_Window;
		bool m_IsRuning = true, m_Minimized = false;
		LayerStack m_LayerStack;
		Timestep m_Timestep;

		float m_LastFrameTime = 0.0f;
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
