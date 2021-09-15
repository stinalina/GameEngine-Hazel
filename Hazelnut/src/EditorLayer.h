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
		   
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_TextureStairs;
		Ref<SubTexture2D> m_TextureTree;
		Ref<SubTexture2D> m_TextureGrass;

		glm::vec4 m_SquareColor = { 0.3f, 0.4f, 0.5f, 1.0f };

		//ParticleSystem m_ParticleSystem;
		//ParticleProps m_Particle;

		uint32_t m_MapWidth, m_MapHeight;
		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
	};
}
