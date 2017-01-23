#ifndef GLACIER_ENGINE_D3D11RENDERER_H_
#define GLACIER_ENGINE_D3D11RENDERER_H_

#include "renderer.h"
#include "D3D11_context.h"

namespace Glacier
{
	class D3D11Renderer : public Renderer {
	protected:
		void draw(Mesh* mesh, const Material& material, const Mat4f& model) noexcept override;
	
	public:
		D3D11Renderer() = default;

		bool initialize() noexcept override;

		void draw(std::vector<RenderingComponent*>& rendering_components, float delta_time) override;

	};
}

#endif //GLACIER_ENGINE_D3D11RENDERER_H_