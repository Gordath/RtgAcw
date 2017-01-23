#ifndef GLACIER_RENDERING_COMPONENT_H_
#define GLACIER_RENDERING_COMPONENT_H_
#include "component.h"
#include "../rendering/include/mesh.h"
#include "object.h"
#include "../rendering/include/material.h"

namespace Glacier
{
	class RenderingComponent : public Component {
	private:
		Mesh* m_mesh;
		
		Material m_material;

		bool m_should_draw;

		bool m_casts_shadows;

	public:
		explicit RenderingComponent(Object* parent) 
		: Component{ "co_rendering", parent },
		m_mesh{nullptr},
		m_should_draw{true},
		m_casts_shadows{true}
		{	
		}

		RenderingComponent(const RenderingComponent& other) = default;
		RenderingComponent& operator=(const RenderingComponent& other) = default;

		void setup() noexcept override;

		void update(float dt, long time) noexcept override;

		void teardown() noexcept override;

		void set_mesh(Mesh* mesh) noexcept
		{
			m_mesh = mesh;
		}

		Mesh* get_mesh() const noexcept
		{
			return m_mesh;
		}

		void set_material(const Material& material) noexcept
		{
			m_material = material;
		}

		const Material& get_material() const noexcept
		{
			return m_material;
		}

		bool should_draw() const noexcept
		{
			return m_should_draw;
		}

		const Vec3f& get_position() const noexcept
		{
			return get_parent()->get_position();
		}

		const Vec3f& get_euler_angles() const noexcept
		{
			return get_parent()->get_euler_angles();
		}

		const Vec3f& get_scale() const noexcept
		{
			return get_parent()->get_scale();
		}

		const Mat4f& get_xform() const noexcept
		{
			return get_parent()->get_xform();
		}

		void set_casts_shadows(bool state) noexcept
		{
			m_casts_shadows = state;
		}

		bool casts_shadows() const noexcept
		{
			return m_casts_shadows;
		}
	};
}

#endif //GLACIER_RENDERING_COMPONENT_H_
