#include "hzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

//#define IMGUI_IMPL_API
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Hazel/Core/Application.h"

#include <GLFW/glfw3.h>

#include "ImGuizmo.h"

namespace Hazel
{
	ImGuiLayer::ImGuiLayer() :
		Layer("ImGuiLayer")
	{ }

	void ImGuiLayer::OnAttach()
	{
        HZ_PROFILE_FUNCTION();

		// Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable multi-Viewport

        // Setup Dear ImGui style
        io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 20.0f);  //TODO:: Make enum for fonts!
        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 20.0f); //make this in if darkTheme...
        //io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Balsamiq/BalsamiqSans-Regular.ttf", 18.0f);

        SetColorTheme(ColorTheme::Dark);

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
	}

    void ImGuiLayer::OnDetech()
    {
        HZ_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnEvent(Event& e)
    {
        if (m_BlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

    void ImGuiLayer::Begin()
    {
        HZ_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::End()
    {
        HZ_PROFILE_FUNCTION();

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::SetColorTheme(ColorTheme theme)
    {
        switch (theme)
        {
        case ColorTheme::Dark: SetDarkColorTheme();
            break;
        case ColorTheme::Hazel: SetHazelColorTheme();
            break;
        default: HZ_CORE_INFO("Color theme doesn't supported!");
            break;
        }
    }

    void ImGuiLayer::SetDarkColorTheme()
    {
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }

    void ImGuiLayer::SetHazelColorTheme()
    {
        ImGui::StyleColorsLight();

        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.211f, 0.141f, 0.09f, 1.0f };
        colors[ImGuiCol_Text] = ImVec4{ 0.894f, 0.803f, 0.705f, 1.0f };
        
        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.839f, 0.705f, 0.560f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.839f, 0.705f, 0.560f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.839f, 0.705f, 0.560f, 1.0f };
        
        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f };
        
        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.211f, 0.141f, 0.09f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.278f, 0.188f, 0.121f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.278f, 0.188f, 0.121f, 1.0f };
        
        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.847f, 0.73f, 0.65f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.847f, 0.73f, 0.65f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.847f, 0.73f, 0.65f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.847f, 0.73f, 0.65f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.847f, 0.73f, 0.65f, 1.0f };
        
        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };

        colors[ImGuiCol_Border] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
        colors[ImGuiCol_Separator] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
        
    }
}
