#pragma once

#include "entt.hpp"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Renderer/EditorCamera.h"

class b2World; //forward declaration

namespace Hazel
{
	class Entity; //forward declaration

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();

		void DuplicateEntity(Entity entity);

		static Ref<Scene> Copy(Ref<Scene> scene);

	private:
		entt::registry m_Registry; // Container for all Entities

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}