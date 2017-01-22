#ifndef GLACIER_SCENE_H_
#define GLACIER_SCENE_H_
#include <vector>
#include "internal/types.h"

namespace Glacier
{
	class Object;

	class Scene {
	private:
		std::vector<Object*> m_objects;

	public:
		Scene() = default;

		virtual ~Scene();

		virtual void initialize() = 0;

		void add_object(Object* object) noexcept;

		const std::vector<Object*>& get_objects() const noexcept
		{
			return m_objects;
		}

		virtual void on_key_down(unsigned char key, int x, int y) noexcept = 0;

		virtual void on_key_up(unsigned char key, int x, int y) noexcept = 0;

		virtual void on_mouse_motion(int x, int y) noexcept = 0;

		virtual void on_mouse_click(int button, bool state, int x, int y) = 0;

		virtual void update(float delta_time, long time = 0) noexcept;

		virtual void draw() const noexcept = 0;

		virtual void on_message(MessageContainer msg) const noexcept;
	};
}

#endif //GLACIER_SCENE_H_
