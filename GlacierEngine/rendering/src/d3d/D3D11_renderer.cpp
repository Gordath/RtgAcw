#include "D3D11_renderer.h"
#include "internal/engine_context.h"
#include "../../../../include/main_scene.h"
#include <iostream>
#include <minwinbase.h>
#include "light_component.h"

namespace Glacier
{
	

	bool D3D11Renderer::initialize() noexcept
	{
		

		return true;
	}

	void D3D11Renderer::draw(std::vector<RenderingComponent*>& rendering_components, float delta_time)
	{
		Renderer::draw(rendering_components, delta_time);
	}

	void D3D11Renderer::draw(Mesh* mesh, const Material& material, const Mat4f& model) noexcept
	{
		

		

//		device_context->OMSetRenderTargets(1, GAPI_context->get_address_of_render_target_view(), GAPI_context->get_default_depth_stencil_view());

		
	}
}
