#include "hzpch.h"
#include "Scene.h"

#include "Component.h"
#include "Hazel/Renderer/Renderer.h"

#include "glm/glm.hpp"

#include "Entity.h"

namespace Hazel
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
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