#ifndef GLACIER_ENGINE_RENDERER_H_
#define GLACIER_ENGINE_RENDERER_H_
#include "rendering_component.h"
#include <algorithm>

namespace Glacier
{
	class Renderer {
	protected:
		virtual void draw(Mesh* mesh, const Material& material, const Mat4f& model) noexcept = 0;

	public:
		virtual ~Renderer() = default;

		virtual bool initialize() noexcept = 0;

		virtual void draw(std::vector<RenderingComponent*>& rendering_components, float delta_time)
		{
			std::sort(rendering_components.begin(),
			          rendering_components.end(),
			          [](auto a, auto b) {
				          return a->get_material().diffuse.w > b->get_material().diffuse.w;
			          }
			);

			for (const auto rendering_component : rendering_components) {
				if (rendering_component->should_draw() && rendering_component->get_mesh()) {
					draw(rendering_component->get_mesh(), rendering_component->get_material(), rendering_component->get_xform());
				}
			}
		}
	};
}

#endif //GLACIER_ENGINE_RENDERER_H_
