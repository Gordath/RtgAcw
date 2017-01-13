#include "emitter_conponent.h"

namespace Glacier
{
	void EmitterComponent::setup() noexcept
	{
	}

	void EmitterComponent::update(float dt, long time) noexcept
	{
		auto it{ m_particles.begin() };
		while (it != m_particles.end()) {
			Particle& p{ *it };

			if (!p.active) {
				m_particles.erase(it);
			}
		}
	}
	
	void EmitterComponent::teardown() noexcept
	{
	}
}
