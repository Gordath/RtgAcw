#include "light_component.h"
#include "object.h"

namespace Glacier
{
	void LightComponent::setup() noexcept
	{
	}

	void LightComponent::update(float dt, long time) noexcept
	{
		m_light_desc.position = (m_parent->get_xform() * Vec4f{0.0f, 0.0f, 0.0f, 1.0f}).xyz;
		m_light_desc.light_view_matrix = MathUtils::lookat_lh(m_light_desc.position, Vec3f{0.0f, 0.0f, -1.1f}, Vec3f{ 0.0f, 1.0f, 0.0f });
	}

	void LightComponent::teardown() noexcept
	{
	}
}
