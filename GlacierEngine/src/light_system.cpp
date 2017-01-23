#include "light_system.h"

namespace Glacier
{
	void LightSystem::update_active_light_descriptions() const
	{
		m_active_light_descriptions.clear();

		for (auto entry : m_lights) {
			LightComponent* light_component{ static_cast<LightComponent*>(entry.second->get_component("co_light")) }; // parasoft-suppress  OOP-49 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-29 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-35 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable."
			m_active_light_descriptions.push_back(light_component->get_light_description());
		}
	}

	bool LightSystem::initialize() noexcept
	{
		return true;
	}

	std::vector<LightDesc> LightSystem::get_active_light_descriptions() const noexcept
	{
		std::vector<LightDesc> ldescs;

		for (auto light : m_lights) {
			LightComponent* lc{ static_cast<LightComponent*>(light.second->get_component("co_light")) }; /* parasoft-suppress  OOP-49 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." */ /* parasoft-suppress  OOP-29 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." */ /* parasoft-suppress  OOP-35 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." */
			ldescs.push_back(lc->get_light_description());
		}

		return ldescs;
	}

	void LightSystem::process(const std::vector<Object*>& objects, float delta_time) const noexcept
	{
		static bool lights_added{ false };

		for (auto object : objects) {
			LightComponent* light_component{ static_cast<LightComponent*>(object->get_component("co_light")) }; // parasoft-suppress  OOP-49 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-29 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-35 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable."

			if (light_component) {
				Object* light{ m_lights[object->get_name()] };

				if (!light) {
					m_lights[object->get_name()] = object;
//					m_active_light_descriptions.push_back(light_component->get_light_description());
//					lights_added = true;
				}
			}
		}

		if (lights_added) {
//			update_active_light_descriptions();
//			lights_added = false;
		}
	}
}
