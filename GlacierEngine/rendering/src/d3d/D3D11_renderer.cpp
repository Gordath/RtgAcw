#include "D3D11_renderer.h"
#include "internal/engine_context.h"

namespace Glacier
{
	bool D3D11Renderer::initialize() noexcept
	{
		return true;
	}

	void D3D11Renderer::draw(RenderingComponent* rendering_component, float delta_time)
	{
		D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

		float cl_col[4]{ 0.2f, 0.2f, 0.2f, 0.0f };
		device_context->ClearRenderTargetView(GAPI_context->get_default_render_target_view(), cl_col);
		device_context->ClearDepthStencilView(GAPI_context->get_default_depth_stencil_view(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}
