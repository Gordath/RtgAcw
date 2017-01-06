#include "scene_manager.h"

namespace Glacier
{
	std::vector<Scene*> SceneManager::m_scenes;

	SceneManager::~SceneManager()
	{
		for (auto scene : m_scenes) {
			delete scene;
		}
	}

	void SceneManager::push_scene(Scene* scene) noexcept
	{
		scene->initialize();
		m_scenes.push_back(scene);
	}

	Scene* SceneManager::pop_scene() noexcept
	{
		Scene* sc{ m_scenes.back() };

		m_scenes.erase(m_scenes.end());

		return sc;
	}

	void SceneManager::on_key_down(unsigned char key, int x, int y) noexcept
	{
		if (m_scenes.back()) {
			m_scenes.back()->on_key_down(key, x, y);
		}
	}

	void SceneManager::on_key_up(unsigned char key, int x, int y) noexcept
	{
		if (m_scenes.back()) {
			m_scenes.back()->on_key_up(key, x, y);
		}
	}

	void SceneManager::on_mouse_motion(int x, int y) noexcept
	{
		if (m_scenes.back()) {
			m_scenes.back()->on_mouse_motion(x, y);
		}
	}

	void SceneManager::on_mouse_click(int button, bool state, int x, int y) noexcept
	{
		if (m_scenes.back()) {
			m_scenes.back()->on_mouse_click(button, state, x, y);
		}
	}

	void SceneManager::update(float delta_time, long time) noexcept
	{
		if (m_scenes.back()) {
			m_scenes.back()->update(delta_time, time);
		}
	}

	void SceneManager::draw() noexcept
	{
		if (m_scenes.back()) {
			m_scenes.back()->draw();
		}
	}
}
