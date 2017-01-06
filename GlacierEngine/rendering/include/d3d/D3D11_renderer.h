#ifndef GLACIER_ENGINE_D3D11RENDERER_H_
#define GLACIER_ENGINE_D3D11RENDERER_H_

#include "renderer.h"
#include "D3D11_context.h"

namespace Glacier
{
	class D3D11Renderer : public Renderer {
	private:
		ComPtr<ID3D11Buffer> m_uniform_buffer;
		ComPtr<ID3D11Buffer> m_light_structured_buffer;
		ComPtr<ID3D11ShaderResourceView> m_light_srv;

	protected:
		void draw(Mesh* mesh, const Mat4f& model) noexcept override;
	
	public:
		bool initialize() noexcept override;

		void draw(const std::vector<RenderingComponent*>& rendering_components, float delta_time) override;

	};
}

#endif //GLACIER_ENGINE_D3D11RENDERER_H_