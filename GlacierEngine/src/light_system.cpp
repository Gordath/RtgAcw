#include "light_system.h"

namespace Glacier
{
	void LightSystem::update_active_light_descriptions() const
	{
		m_active_light_descriptions.clear();

		for (auto entry : m_lights) {
			LightComponent* light_component{ static_cast<LightComponent*>(entry.second->get_component("co_light")) };
			m_active_light_descriptions.push_back(light_component->get_light_description());
		}
	}

	bool LightSystem::initialize() noexcept
	{
		return true;
	}

	void LightSystem::process(const std::vector<Object*>& objects, float delta_time) const noexcept
	{
		static bool lights_added{ false };

		for (auto object : objects) {
			LightComponent* light_component{ static_cast<LightComponent*>(object->get_component("co_light")) };

			if (light_component) {
				Object* light{ m_lights[object->get_name()] };

				if (!light) {
					m_lights[object->get_name()] = object;
					m_active_light_descriptions.push_back(light_component->get_light_description());
					lights_added = true;
				}
			}
		}

		if (lights_added) {
			update_active_light_descriptions();
			lights_added = false;
		}
	}
}
