#ifndef GLACIER_RENDER_SYSTEM_H_
#define GLACIER_RENDER_SYSTEM_H_
#include <memory>
#include "renderer.h"
#include "system.h"
#include "../rendering/include/d3d/D3D11_renderer.h"

namespace Glacier
{
	enum class RendererType {
		D3D_FORWARD_RENDERER,
		D3D_DEFERED_RENDERER
	};

	class RenderSystem : public System {
	private:
		std::unique_ptr<Renderer> m_renderer {std::make_unique<D3D11Renderer>()};

	public:
		bool initialize() noexcept override;

		void process(const std::vector<Object*>& objects, float delta_time) const noexcept override;
	};
}

#endif //GLACIER_RENDER_SYSTEM_H_
