#include "hzpch.h"
#include "Scene.h"

#include "Component.h"
#include "Hazel/Renderer/Renderer.h"

#include "glm/glm.hpp"

namespace Hazel
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{

	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transorm, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer::DrawQuad(transorm, sprite.Color);
		}
	}
}