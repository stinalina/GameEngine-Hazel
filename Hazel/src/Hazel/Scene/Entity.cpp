#include "hzpch.h"
#include "Entity.h"

namespace Hazel
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
		m_ID = (uint64_t)m_EntityHandle; //entt just increase the count of entitys, so the first entity has id 0
	}
}