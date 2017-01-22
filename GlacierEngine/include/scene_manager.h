#ifndef GLACIER_SCENE_MANAGER_H_
#define GLACIER_SCENE_MANAGER_H_
#include <vector>
#include "scene.h"

namespace Glacier
{
	class SceneManager {
	private:
		static std::vector<Scene*> m_scenes;

	public:
		SceneManager() = default;

		~SceneManager();

		static void push_scene(Scene* scene) noexcept;

		static Scene* pop_scene() noexcept;

		static void on_key_down(unsigned char key, int x, int y) noexcept;

		static void on_key_up(unsigned char key, int x, int y) noexcept;

		static void on_mouse_motion(int x, int y) noexcept;

		static void on_mouse_click(int button, bool state, int x, int y) noexcept;

		// Respond to messages
		static void on_message(MessageContainer msg) noexcept;

		static void update(float delta_time, long time) noexcept;

		static void draw() noexcept;
	};
}

#endif //GLACIER_SCENE_MANAGER_H_
