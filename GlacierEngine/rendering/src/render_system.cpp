#include "render_system.h"

namespace Glacier
{
	void RenderSystem::process(const std::vector<Object*>& objects, float delta_time) const noexcept
	{
		std::vector<RenderingComponent*> rendering_components;
		for (const auto obj : objects) {
			RenderingComponent* rc{ static_cast<RenderingComponent*>(obj->get_component("co_rendering")) };

			if (rc) {
				rendering_components.push_back(rc);
			}
		}

		m_renderer->draw(nullptr, delta_time);

		//TODO: figure out how this is going to work with the render passes

	}
}
