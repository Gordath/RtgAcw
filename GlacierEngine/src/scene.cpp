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
			auto object{ *it };
			if (!object->is_alive()) {
				delete object;
				it = m_objects.erase(it);
			} else {
				object->update(delta_time, time);
				++it;
			}
		}
	}
}
