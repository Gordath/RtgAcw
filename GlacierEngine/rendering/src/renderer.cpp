#include "renderer.h"

namespace Glacier
{
	Renderer::~Renderer()
	{
	}

	void Renderer::draw(std::vector<RenderingComponent*>& rendering_components, float delta_time)
	{


		for (const auto rendering_component : rendering_components) {
			if (rendering_component->should_draw() && rendering_component->get_mesh()) {
				draw(rendering_component->get_mesh(), rendering_component->get_material(), rendering_component->get_xform());
			}
		}
	}
}
