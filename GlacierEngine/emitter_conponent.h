#ifndef GLACIER_EMITTER_COMPONENT_H_
#define GLACIER_EMITTER_COMPONENT_H_
#include "component.h"
#include "internal/types.h"
#include <vector>

namespace Glacier
{
	struct Particle {
		Vec3f position;
		Vec4f color;
		float size;

		Vec3f velocity;
		float life;
		bool active;

		double spawn_time;
	};

	class EmitterComponent : public Component {
	private:
		std::vector<Particle> m_particles;

		float m_spawn_rate{ 1.0f };
		float m_lifespan{ 1.0f };
		int m_max_particles{ 1 };
		float m_spawn_radius{ 0.0f };

		Vec4f m_start_color;
		Vec4f m_end_color;

		float m_particles_to_spawn{ 0.0f };

		bool m_active{ true };

	public:
		EmitterComponent(Object* parent) : Component{ "co_emitter", parent }
		{
		}


		float get_spawn_rate() const noexcept
		{
			return m_spawn_rate;
		}

		void set_spawn_rate(float spawn_rate) noexcept
		{
			m_spawn_rate = spawn_rate;
		}

		float get_lifespan() const noexcept
		{
			return m_lifespan;
		}

		void set_lifespan(float lifespan) noexcept
		{
			m_lifespan = lifespan;
		}

		int get_max_particles() const noexcept
		{
			return m_max_particles;
		}

		void set_max_particles(int max_particles) noexcept
		{
			m_max_particles = max_particles;
		}

		float get_spawn_radius1() const noexcept
		{
			return m_spawn_radius;
		}

		void set_spawn_radius(float spawn_radius) noexcept
		{
			m_spawn_radius = spawn_radius;
		}

		const Vec4f& get_start_color() const noexcept
		{
			return m_start_color;
		}

		void set_start_color(const Vec4f& start_color) noexcept
		{
			m_start_color = start_color;
		}

		const Vec4f& get_end_color() const noexcept
		{
			return m_end_color;
		}

		void set_end_color(const Vec4f& end_color) noexcept
		{
			m_end_color = end_color;
		}

		bool is_active() const noexcept
		{
			return m_active;
		}

		void set_active(bool active) noexcept
		{
			m_active = active;
		}


		void setup() noexcept override;
		void update(float dt, long time) noexcept override;
		void teardown() noexcept override;
	};
}

#endif //GLACIER_EMITTER_COMPONENT_H_
