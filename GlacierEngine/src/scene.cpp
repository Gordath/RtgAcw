#include "scene.h"

namespace Glacier
{
	Scene::~Scene()
	{
		for (const auto object : m_objects) {
			delete object;
		}

		m_objects.clear();
	}

	void Scene::update(float delta_time, long time) noexcept
	{
		auto it{ m_objects.begin() };
		
		while (it != m_objects.end()) {
			auto game_object{ *it };
			if (!game_object->is_alive()) {
				delete game_object;
				it = m_objects.erase(it);
			} else {
				//TODO: maybe update the game objects here if they actually need an update method.
				++it;
			}
		}
	}
}
