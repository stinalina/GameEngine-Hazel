#include "hzpch.h"
#include "Scene.h"

#include "Components.h"
#include "ScriptableEntity.h" //need this because componenets don't include this one

#include "Hazel/Renderer/Renderer.h"

#include "glm/glm.hpp"

#include "Entity.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Hazel
{
	static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		HZ_CORE_ASSERT(false, "Unknown body type");
		return b2_staticBody;
	}

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<uint64_t, entt::entity>& enttMap)
	{
		([&]()
		{
			auto view = src.view<Component>();
			for (auto srcEntity : view)
			{
				entt::entity dstEntity = enttMap.at((uint64_t)srcEntity);;

				auto& srcComponent = src.get<Component>(srcEntity);
				dst.emplace_or_replace<Component>(dstEntity, srcComponent);
			}
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<uint64_t, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
		{
			if (src.HasComponent<Component>())
				dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	static void CopyAllComponents(entt::registry& dstSceneRegistry, entt::registry& srcSceneRegistry, const std::unordered_map<uint64_t, entt::entity>& enttMap)
	{
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);
	}

	static void CopyAllExistingComponents(Entity dst, Entity src)
	{
		CopyComponentIfExists(AllComponents{}, dst, src);
	}

	Ref<Scene> Scene::Copy(Ref<Scene> scene)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = scene->m_ViewportWidth;
		newScene->m_ViewportHeight = scene->m_ViewportHeight;

		std::unordered_map<uint64_t, entt::entity> enttMap;

		// Create entities in new scene
		auto& srcSceneRegistry = scene->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		auto idView = srcSceneRegistry.view<TransformComponent>(); //because every valid entity has a transform
		for (auto e : idView)
		{
			auto uuid = (uint64_t)e;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntity(name);
			enttMap[uuid] = newEntity;
		}

		CopyAllComponents(dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		//Scripts
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			if (!nsc.Instance)
			{
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->m_Entity = Entity{ entity, this };
				nsc.Instance->OnCreate();
			}

			nsc.Instance->OnUpdate(ts);
		});

		//Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations); //how oft we recalculate physics during a frame

			// Retrieve transform from Box2D
			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
		}

		//Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) //find main camera
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
		
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		
			//if we found the mainCamera -> Render stuff
			if (mainCamera)
			{
				Renderer::BeginScene(mainCamera->GetProjection(), cameraTransform);

				//Render Sprites
				//auto spriteGroup = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				//for (auto entity : spriteGroup)
				//{
				//	auto [transform, sprite] = spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);
				//	Renderer::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				//}

				auto spriteView = m_Registry.view<TransformComponent, SpriteRendererComponent>();
				for (auto entity : spriteView)
				{
					auto [transform, sprite] = spriteView.get<TransformComponent, SpriteRendererComponent>(entity);
					Renderer::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				}

				auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, circleRenderer] = view.get<TransformComponent, CircleRendererComponent>(entity);

					Renderer::DrawCircle(transform.GetTransform(), circleRenderer.Color, circleRenderer.Thickness, (int)entity);
				}
				
				//Render Particles
				//auto particleGroup = m_Registry.group<TransformComponent>(entt::get<ParticleSystemComponent>);
				//for (auto entity : particleGroup)
				//{
				//	auto [transform, particles] = particleGroup.get<TransformComponent, ParticleSystemComponent>(entity);
				//	particles.Particle.Position = glm::vec2(transform.Translation.x, transform.Translation.y);
				//	particles.Instance.OnUpdate(ts);
				//	particles.Instance.OnRender(*mainCamera);
				//}
				
				//TODO:: This is not working. Can't find any ParticleSystemComponent!?

				auto particleView = m_Registry.view<TransformComponent, ParticleSystemComponent>();
				for (auto entity : particleView)
				{
					HZ_CORE_INFO("Found some particle to render!");
					auto [transform, particles] = particleView.get<TransformComponent, ParticleSystemComponent>(entity);
					particles.Particle.Position = glm::vec2(transform.Translation.x, transform.Translation.y);
					particles.Instance.OnUpdate(ts);
					particles.Instance.OnRender(*mainCamera);
				}

				Renderer::EndScene();
			}
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
		for (auto entity : view)
		{
			auto [transform, circleRenderer] = view.get<TransformComponent, CircleRendererComponent>(entity);

			Renderer::DrawCircle(transform.GetTransform(), circleRenderer.Color, circleRenderer.Thickness, (int)entity);
		}

		Renderer::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) //Resize! 
				cameraComponent.Camera.SetViewportSize(width, height); 
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateEntity(entity.GetName());
		CopyAllExistingComponents(newEntity, entity);
	}
}