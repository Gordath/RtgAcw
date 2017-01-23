#include "emitter_conponent.h"
#include "internal/math_utils.h"
#include <algorithm>
#include "object.h"
#include <iostream>

namespace Glacier
{
#undef min
#undef max

	void EmitterComponent::setup() noexcept
	{
	}

	void EmitterComponent::update(float dt, long time) noexcept
	{
		double tsec{ time / 1000.0 };

		auto it{ m_particles.begin() };
		while (it != m_particles.end()) {
			auto& p = *it;

			if (!p.active) {
				it = m_particles.erase(it);
				continue;
			}

			double t{ (tsec - p.spawn_time) / m_lifespan };

			float min_x = std::min(m_start_color.x, m_end_color.x);
			float max_x = std::max(m_start_color.x, m_end_color.x);

			float min_y = std::min(m_start_color.y, m_end_color.y);
			float max_y = std::max(m_start_color.y, m_end_color.y);

			float min_z = std::min(m_start_color.z, m_end_color.z);
			float max_z = std::max(m_start_color.z, m_end_color.z);

			float min_w = std::min(m_start_color.w, m_end_color.w);
			float max_w = std::max(m_start_color.w, m_end_color.w);

			Vec4f col;

			col.x = m_start_color.x + (m_end_color.x - m_start_color.x) * t;
			col.y = m_start_color.y + (m_end_color.y - m_start_color.y) * t;
			col.z = m_start_color.z + (m_end_color.z - m_start_color.z) * t;
			col.w = m_start_color.w + (m_end_color.w - m_start_color.w) * t;

			col.x = std::max(min_x, std::min(col.x, max_x));
			col.y = std::max(min_y, std::min(col.y, max_y));
			col.z = std::max(min_z, std::min(col.z, max_z));
			col.w = std::max(min_w, std::min(col.w, max_w));

			p.color = col;

			//simulate physics.
			p.position += p.velocity * dt;
			p.velocity += m_external_force * dt;
			p.life -= dt;

			if (p.life < 0.0f) {
				p.active = false;
			}

			++it;
		}

		//Spawn particles
		m_particles_to_spawn += m_spawn_rate * dt;
		int num_spawn = m_particles_to_spawn;
		m_particles_to_spawn -= num_spawn;

		int i = 0;

		if (m_active) {
			while (i < num_spawn && m_particles.size() < m_max_particles) {
				
				float rand_pos_x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_spawn_radius * 2.0f - m_spawn_radius;
				float rand_pos_y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_spawn_radius * 2.0f - m_spawn_radius;
				float rand_pos_z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_spawn_radius * 2.0f - m_spawn_radius;

				float rand_vel_x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_velocity_range * 2.0f - m_velocity_range;
				float rand_vel_y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_velocity_range * 2.0f - m_velocity_range;
				float rand_vel_z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_velocity_range * 2.0f - m_velocity_range;
				
				Particle p;
				p.position = (get_parent()->get_xform() * Vec4f{0.0f, 0.0f, 0.0f, 1.0f}).xyz + Vec3f{ rand_pos_x, rand_pos_y, rand_pos_z };
				p.velocity = m_velocity + Vec3f{ rand_vel_x, rand_vel_y, rand_vel_z };
				p.life = m_lifespan;
				p.color = m_start_color;
				p.spawn_time = tsec;
				p.size = m_particle_size;
				p.active = true;

				m_particles.push_back(p);

				++i;
			}
		}
	}
	
	void EmitterComponent::teardown() noexcept
	{
	}
}
