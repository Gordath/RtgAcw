#ifndef GLACIER_EMITTER_COMPONENT_H_
#define GLACIER_EMITTER_COMPONENT_H_
#include "component.h"
#include "internal/types.h"
#include <vector>
#include "rendering_component.h"

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

		float m_particle_size{ 1.0f };

		Vec4f m_start_color;
		Vec4f m_end_color;

		float m_particles_to_spawn{ 0.0f };

		bool m_active{ true };

		//Physics
		Vec3f m_velocity;
		float m_velocity_range{ 0.0f };
		Vec3f m_external_force;

		//Drawing info
		Mesh* m_mesh{ nullptr };
		Material m_material;

	public:
		EmitterComponent(Object* parent) : Component{ "co_emitter", parent }
		{
		}

		const std::vector<Particle>& get_particles() const noexcept
		{
			return m_particles;
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

		float get_spawn_radius() const noexcept
		{
			return m_spawn_radius;
		}

		void set_spawn_radius(float spawn_radius) noexcept
		{
			m_spawn_radius = spawn_radius;
		}

		float get_particle_size() const noexcept
		{
			return m_particle_size;
		}

		void set_particle_size(float particle_size) noexcept
		{
			m_particle_size = particle_size;
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

		const Vec3f& get_velocity() const noexcept
		{
			return m_velocity;
		}

		void set_velocity(const Vec3f& velocity) noexcept
		{
			m_velocity = velocity;
		}

		float get_velocity_range() const noexcept
		{
			return m_velocity_range;
		}

		void set_velocity_range(float velocity_range) noexcept
		{
			m_velocity_range = velocity_range;
		}

		const Vec3f& get_external_force() const noexcept
		{
			return m_external_force;
		}

		void set_external_force(const Vec3f& external_force) noexcept
		{
			m_external_force = external_force;
		}

		Mesh* get_mesh() const noexcept
		{
			return m_mesh;
		}

		void set_mesh(Mesh* mesh) noexcept
		{
			m_mesh = mesh;
		}

		const Material& get_material() const noexcept
		{
			return m_material;
		}

		void set_material(const Material& material) noexcept
		{
			m_material = material;
		}

		void setup() noexcept override;

		void update(float dt, long time) noexcept override;

		void teardown() noexcept override;
	};
}

#endif //GLACIER_EMITTER_COMPONENT_H_
