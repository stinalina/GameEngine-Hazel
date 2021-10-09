#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Hazel
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...); //"just pass the args further to the entt script, which will handle it for us"
		}

		template<typename T>
		T& GetComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not has component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent() { return m_Scene->m_Registry.all_of<T>(m_EntityHandle); }

		template<typename T>
		void RemoveComponent()
		{
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not has component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}
		
		uint64_t GetID() { return m_ID; }

		//Operators
		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		bool operator==(const Entity& other) const{ return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }

		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		uint64_t m_ID; //use entt count for this
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}