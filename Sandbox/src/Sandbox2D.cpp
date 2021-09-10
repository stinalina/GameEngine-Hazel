#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <imgui/imgui.h>
#include <glm/glm/gtc/type_ptr.hpp>
#include "Hazel/Renderer/Texture.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_Texture = Hazel::Texture2D::Create("assets/textures/test.png");
}

void Sandbox2D::OnDetech()
{

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	//Update
	{
		HZ_PROFILE_SCOPE("Camera Controller");
		m_CameraController.OnUpdate(ts);
	}
	
	//Render
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0 });
		Hazel::RenderCommand::Clear();
	}
	{
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());
			Hazel::Renderer::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.5f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			Hazel::Renderer::DrawQuad({ 0.5f, 0.5f }, { 1.0f, 1.0f }, { 0.2f, 0.3f, 0.8f, 1.0f });
			Hazel::Renderer::DrawQuad({ 0.0f, -1.0f, -0.1f }, { 3.f, 3.0f }, m_Texture);
		Hazel::Renderer::EndScene();
	}
	
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}