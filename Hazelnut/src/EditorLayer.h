#pragma once

#include "Hazel.h"

//#include "ParticleSystem.h"

namespace Hazel
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetech() override;

		void OnUpdate(Hazel::Timestep ts);
		virtual void OnImGuiRender() override;
		void OnEvent(Hazel::Event& e) override;

	private:
		Hazel::OrthographicCameraController m_CameraController;

		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		   
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;
		Ref<Hazel::Texture2D> m_Texture;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec4 m_SquareColor = { 0.3f, 0.4f, 0.5f, 1.0f };
	};
}
