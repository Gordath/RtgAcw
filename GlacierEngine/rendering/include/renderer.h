#ifndef GLACIER_ENGINE_RENDERER_H_
#define GLACIER_ENGINE_RENDERER_H_
#include "rendering_component.h"

namespace Glacier
{
	class Renderer {
	protected:
		virtual void draw(Mesh* mesh /*TODO:Add material and shit*/) noexcept = 0;

	public:
		virtual ~Renderer() = default;

		virtual bool initialize() noexcept = 0;

		virtual void draw(const std::vector<RenderingComponent*>& rendering_components, float delta_time)
		{
			for (const auto rendering_component : rendering_components) {
				if (rendering_component->should_draw() && rendering_component->get_mesh()) {
					draw(rendering_component->get_mesh());
				}
			}
		}
	};
}

#endif //GLACIER_ENGINE_RENDERER_H_
