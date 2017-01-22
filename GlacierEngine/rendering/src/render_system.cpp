#include "render_system.h"
#include <iostream>

namespace Glacier
{
	bool RenderSystem::initialize() noexcept
	{
		if (!m_renderer->initialize()) {
			std::cerr << "Render system initialization failed." << std::endl;
			return false;
		}

		return true;
	}

	void RenderSystem::process(const std::vector<Object*>& objects, float delta_time) const noexcept
	{
		std::vector<RenderingComponent*> rendering_components;
		for (const auto obj : objects) {
			RenderingComponent* rc{ static_cast<RenderingComponent*>(obj->get_component("co_rendering")) }; // parasoft-suppress  OOP-35 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable."

			if (rc) {
				rendering_components.push_back(rc);
			}
		}

		m_renderer->draw(rendering_components, delta_time);
	}
}
