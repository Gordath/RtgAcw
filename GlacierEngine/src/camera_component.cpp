#include "camera_component.h"
#include "object.h"

namespace Glacier
{
	void CameraComponent::setup() noexcept
	{
	}

	void CameraComponent::update(float dt, long time) noexcept
	{
	}

	void CameraComponent::teardown() noexcept
	{
	}

	Mat4f CameraComponent::get_view_matrix() const noexcept
	{
		return MathUtils::inverse(m_parent->get_xform());
	}

	Mat4f CameraComponent::get_projection_matrix() const noexcept
	{		
		return MathUtils::perspective_lh(m_fov, m_width, m_height, m_near_plane, m_far_plane);
	}
}
