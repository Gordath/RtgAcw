#include "camera_system.h"
#include "camera_component.h"

namespace Glacier
{
	bool CameraSystem::initialize() noexcept
	{
		return true;
	}

	void CameraSystem::process(const std::vector<Object*>& objects, float delta_time) const noexcept
	{
		for (auto object : objects) {
			CameraComponent* camera_component{ static_cast<CameraComponent*>(object->get_component("co_camera")) }; // parasoft-suppress  OOP-49 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-29 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-35 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable."

			if (camera_component) {
				Object* cam{ m_cameras[object->get_name()] };

				if (!cam) {
					m_cameras[object->get_name()] = object;

					if (m_cameras.size() == 1) {
						m_active_camera = object;
					}
				}
			}
		}
	}

	void CameraSystem::set_active_camera(const std::string name) const noexcept
	{
		m_active_camera = m_cameras[name];
	}

	Object* CameraSystem::get_active_camera() const noexcept
	{
		return m_active_camera;
	}

	Mat4f CameraSystem::get_active_camera_view_matrix() const noexcept
	{
		if (m_active_camera) {
			CameraComponent* camera_component{ static_cast<CameraComponent*>(m_active_camera->get_component("co_camera")) }; // parasoft-suppress  OOP-49 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-29 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-35 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable."

			return camera_component->get_view_matrix();
		}

		return Mat4f{};
	}

	Mat4f CameraSystem::get_active_camera_projection_matrix() const noexcept
	{
		if (m_active_camera) {
			CameraComponent* camera_component{ static_cast<CameraComponent*>(m_active_camera->get_component("co_camera")) }; // parasoft-suppress  OOP-49 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-29 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-35 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable."

			return camera_component->get_projection_matrix();
		}

		return Mat4f{};
	}
}
