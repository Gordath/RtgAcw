#ifndef GLACIER_SCENE_H_
#define GLACIER_SCENE_H_
#include <vector>
#include <memory>
#include "object.h"

namespace Glacier
{
	class Scene {
	protected:
		std::vector<Object*> m_objects;

	public:
		virtual ~Scene();

		virtual void initialize() = 0;

		virtual void on_key_down(int key, int x, int y) noexcept = 0;

		virtual void on_key_up(int key, int x, int y) noexcept = 0;

		// Respond to messages
		//virtual void OnMessage(Message* msg);

		virtual void update(float delta_time, long time) noexcept;

		virtual void draw() const noexcept = 0;
	};
}

#endif //GLACIER_SCENE_H_
