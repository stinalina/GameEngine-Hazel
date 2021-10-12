#pragma once
#include "Hazel/Core/Timestep.h"
#include "Hazel/Renderer/Camera.h"

namespace Hazel
{
	struct ParticleProps
	{
		glm::vec2 Position; //TODO:: This one will have the positions of it's entity object!
		glm::vec2 Velocity = { 0.0f, 0.0f },  VelocityVariation = { 3.0f, 1.0f };

		glm::vec4 ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };;
		glm::vec4 ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };;

		float SizeBegin = 0.5f, SizeEnd = 0.0f, SizeVariation = 0.3f;
		float LifeTime = 1.0f;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem(uint32_t maxParticles = 100000);

		void OnUpdate(Timestep ts);
		void OnRender(Camera& camera);

		void Emit(const ParticleProps& particleProps);

	private:
		struct Particle
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};
		std::vector<Particle> m_ParticlePool;

		uint32_t m_PoolIndex;
	};
}