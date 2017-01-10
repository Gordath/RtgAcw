#ifndef GLACIER_CAMERA_COMPONENT_H_
#define GLACIER_CAMERA_COMPONENT_H_
#include "component.h"
#include "internal/types.h"

namespace Glacier
{
	class CameraComponent : public Component {
	private:
		float m_fov{ 0.0f };
		float m_width{ 0.0f };
		float m_height{ 0.0f };
		float m_near_plane{ 0.0f };
		float m_far_plane{ 0.0f };

	public:
		CameraComponent(Object* parent,
		                float fov,
		                float width,
		                float height,
		                float near_plane,
		                float far_plane) : Component{ "co_camera", parent },
		                                   m_fov{ fov },
		                                   m_width{ width },
		                                   m_height{ height },
		                                   m_near_plane{ near_plane },
		                                   m_far_plane{ far_plane }
		{
		}

		void setup() noexcept override;

		void update(float dt, long time) noexcept override;

		void teardown() noexcept override;

		Mat4f get_view_matrix() const noexcept;

		Mat4f get_projection_matrix() const noexcept;
	};
}

#endif //GLACIER_CAMERA_COMPONENT_H_
