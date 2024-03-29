#pragma once

#include "Hazel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Hazel/Renderer/EditorCamera.h"

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
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();

		void DuplicateSelectedEntity();

		void UI_Toolbar();

		OrthographicCameraController m_CameraController;
		EditorCamera m_EditorCamera;

		bool m_ViewportFocused = false,  m_ViewportHovered = false;
		glm::vec2 m_ViewportSize; // = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene, m_RuntimeScene;
		enum class SceneState
		{
			Edit = 0, Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;

		Entity m_HoveredEntity;

		int m_GizmoType = -1;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		//Editor Resources
		Ref<Texture2D> m_IconPlay, m_IconStop;
	};
}
