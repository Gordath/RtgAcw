#ifndef GLACIER_ENGINE_RENDERER_H_
#define GLACIER_ENGINE_RENDERER_H_
#include "rendering_component.h"

namespace Glacier
{
	class Renderer {
	public:
		virtual ~Renderer() = default;

		virtual bool initialize() noexcept = 0;

		virtual void draw(RenderingComponent* rendering_component, float delta_time) = 0;
	};
}

#endif //GLACIER_ENGINE_RENDERER_H_
