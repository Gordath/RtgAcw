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
		Renderer() = default;

		virtual ~Renderer();

		virtual bool initialize() noexcept = 0;

		virtual void draw(std::vector<RenderingComponent*>& rendering_components, float delta_time);
	};
}

#endif //GLACIER_ENGINE_RENDERER_H_
