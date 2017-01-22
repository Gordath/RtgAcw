#include "scene.h"
#include "object.h"

namespace Glacier
{
	Scene::~Scene()
	{
		try {
			for (const auto object : m_objects) {
				delete object;
			}
		}
		catch(...){}

		m_objects.clear();
	}

	void Scene::add_object(Object* object) noexcept
	{
		m_objects.push_back(object);
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

	void Scene::on_message(MessageContainer msg) const noexcept
	{
		for (auto object : m_objects) {
			object->broadcast_message(msg);
		}
	}
}
