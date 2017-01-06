#ifndef GLACIER_RENDERING_COMPONENT_H_
#define GLACIER_RENDERING_COMPONENT_H_
#include "component.h"
#include "mesh.h"
#include "object.h"
#include "material.h"

namespace Glacier
{
	class RenderingComponent : public Component {
	private:
		Mesh* m_mesh{ nullptr };
		
		Material m_material;

		bool m_should_draw{ true };

	public:
		RenderingComponent(Object* parent) : Component{ "co_rendering", parent }
		{	
		}

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
			return m_parent->get_position();
		}

		const Vec3f& get_euler_angles() const noexcept
		{
			return m_parent->get_euler_angles();
		}

		const Vec3f& get_scale() const noexcept
		{
			return m_parent->get_scale();
		}

		const Mat4f& get_xform() const noexcept
		{
			return m_parent->get_xform();
		}
	};
}

#endif //GLACIER_RENDERING_COMPONENT_H_
