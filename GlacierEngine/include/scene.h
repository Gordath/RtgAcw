#ifndef GLACIER_SCENE_H_
#define GLACIER_SCENE_H_
#include <vector>
#include "object.h"

namespace Glacier
{
	class Scene {
	protected:
		std::vector<Object*> m_objects;

	public:
		virtual ~Scene();

		virtual void initialize() = 0;

		virtual void on_key_down(unsigned char key, int x, int y) noexcept = 0;

		virtual void on_key_up(unsigned char key, int x, int y) noexcept = 0;

		virtual void on_mouse_motion(int x, int y) noexcept = 0;

		virtual void on_mouse_click(int button, bool state, int x, int y) = 0;

		// Respond to messages
		//virtual void OnMessage(Message* msg);

		virtual void update(float delta_time, long time = 0) noexcept;

		virtual void draw() const noexcept = 0;

		virtual void on_message(MessageContainer msg) const noexcept;
	};
}

#endif //GLACIER_SCENE_H_
