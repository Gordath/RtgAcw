#ifndef GLACIER_ENGINE_D3D11RENDERER_H_
#define GLACIER_ENGINE_D3D11RENDERER_H_

#include "renderer.h"

namespace Glacier
{
	class D3D11Renderer : public Renderer {
	public:
		bool initialize() noexcept override;

		void draw(RenderingComponent* rendering_component, float delta_time) override;
	};
}

#endif //GLACIER_ENGINE_D3D11RENDERER_H_