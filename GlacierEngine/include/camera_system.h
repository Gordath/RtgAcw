#ifndef GLACIER_CAMERA_SYSTEM_H_
#define GLACIER_CAMERA_SYSTEM_H_
#include "system.h"
#include <map>
#include "object.h"

namespace Glacier
{
	class CameraSystem : public System {
	private:
		mutable std::map<std::string, Object*> m_cameras;

		mutable Object* m_active_camera{ nullptr };

	public:
		CameraSystem() = default;


		CameraSystem(const CameraSystem& other) = delete;
		explicit CameraSystem(CameraSystem&& other) noexcept = delete;
		CameraSystem& operator=(const CameraSystem& other) = delete;
		CameraSystem& operator=(CameraSystem&& other) noexcept = delete;
		bool initialize() noexcept override;

		void process(const std::vector<Object*>& objects, float delta_time) const noexcept override;

		void set_active_camera(const std::string& name) const noexcept;

		Object* get_active_camera() const noexcept;

		Mat4f get_active_camera_view_matrix() const noexcept;

		Mat4f get_active_camera_projection_matrix() const noexcept;
	};
}

#endif //GLACIER_CAMERA_SYSTEM_H_
