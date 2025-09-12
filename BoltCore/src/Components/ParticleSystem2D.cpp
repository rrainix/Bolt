#include "Bolt/btpch.hpp"
#include "Bolt/Components/ParticleSystem2D.hpp"
#include "Bolt/Utils/Random.hpp"

namespace Bolt {
	void ParticleSystem2D::Update(float deltaTime) {
		if (deltaTime == 0.f) return;

		if (m_IsEmitting) {
			float toEmit = EmissionSettings.EmitOverTime * deltaTime + m_EmitAccumulator;
			int emitCount = static_cast<int>(toEmit);
			m_EmitAccumulator = toEmit - emitCount; // Rest behalten
			Emit(emitCount);

			for (auto& burst : m_Bursts) {
				burst.TimeUntilNext += deltaTime;
				if (burst.TimeUntilNext >= burst.Interval) {
					Emit(burst.Count);
					burst.TimeUntilNext = 0;
				}
			}
		}
		if (m_IsSimulating) {
			for (auto& particle : m_Particles) {
				if (ParticleSettings.UseGravity) {
					particle.Velocity += ParticleSettings.Gravity * deltaTime;
				}

				particle.Transform.Position += particle.Velocity * deltaTime;
			}

			m_Particles.erase(std::remove_if(m_Particles.begin(), m_Particles.end(),
				[deltaTime](Particle& p) {
					return (p.LifeTime -= deltaTime) <= 0.f;
				}), m_Particles.end());
		}
	}

	void ParticleSystem2D::Emit(size_t count) {
		if(!m_IsEmitting || count == 0)
			return;

		static Random rand;
		const uint32_t maxParticles = RenderingSettings.MaxParticles;

		if (m_Particles.size() >= maxParticles)
			return;

		m_Particles.reserve(Clamp<uint32_t>(m_Particles.size() + count, 0, maxParticles));

		while (count > 0) {
			Particle particle;
			particle.LifeTime = ParticleSettings.LifeTime;

			Vec2 position{ 0 };
			Vec2 scale{ ParticleSettings.Scale, ParticleSettings.Scale };

			std::visit([&](auto const& s) {
				using T = std::decay_t<decltype(s)>;
				if constexpr (std::is_same_v<T, CircleParams>) {
					position = s.IsOnCircle ? RandomOnCircle(s.Radius, rand) : RandomInCircle(s.Radius, rand);
					particle.Velocity = Normalized(position) * ParticleSettings.Speed;
				}
				else if constexpr (std::is_same_v<T, SquareParams>) {
					position = Vec2(rand.Next(-s.HalfExtends.x, s.HalfExtends.x), rand.Next(-s.HalfExtends.y, s.HalfExtends.y));
				}
				}, Shape);

			if (EmissionSettings.EmissionSpace == Space::World) {
				position += m_EmitterTransform->Position;
				scale *= m_EmitterTransform->Scale;
			}

			particle.Transform.Position = position;
			particle.Transform.Scale = scale;
			particle.Color = ParticleSettings.UseRandomColors ? Bolt::Color(rand.Next(0.f, 1.f), rand.Next(0.f, 1.f), rand.Next(0.f, 1.f)) : RenderingSettings.Color;
			m_Particles.push_back(particle);

			if (m_Particles.size() >= maxParticles)
				break;

			count--;
		}
	}
}