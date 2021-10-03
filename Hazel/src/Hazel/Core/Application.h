#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel 
{
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			HZ_CORE_ASSERT(index < Count, "index out of bounds");
			return Args[index];
		}
	};

	class Application //Singleton
	{
	public:
		Application(const std::string& name = "Hazel Engine", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		static Application& Get() { return *s_Instance; }
		virtual ~Application() = default;

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		//Getter
		Window&						GetWindow() { return *m_Window; }
		ImGuiLayer*					GetImGuiLayer() { return m_ImGuiLayer; }
		ApplicationCommandLineArgs	GetCommandLineArgs() const { return m_CommandLineArgs; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		bool m_Running = true;
		bool m_Minimized = false;

		float m_LastFrameTime = 0.0f;

		ApplicationCommandLineArgs m_CommandLineArgs;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	// To be defined in the Client
	Application* CreateApplication(ApplicationCommandLineArgs args);
}

