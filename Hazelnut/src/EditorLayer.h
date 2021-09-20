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
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		   
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;
		Ref<Hazel::Texture2D> m_Texture;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;

		glm::vec2 m_ViewportSize;
	};
}
