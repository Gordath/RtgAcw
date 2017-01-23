#ifndef GLACIER_ENGINE_D3D_CONTEXT_H_
#define GLACIER_ENGINE_D3D_CONTEXT_H_

#include "../GAPI_context.h"
#include <D3D/d3d11.h>
#include "../../../include/internal/types.h"

namespace Glacier
{
	class D3D11Context : public GAPIContext {
	private:
		bool create_D3D11_device_and_context();

		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_device_context;
		ComPtr<ID3D11Debug> m_debug_interface;

		ComPtr<ID3D11RenderTargetView> m_default_rtv;
		ComPtr<ID3D11DepthStencilView> m_default_dsv;

	public:
		D3D11Context() = default;

		bool create() override;

		ID3D11Device* get_device() const;
		ID3D11DeviceContext* get_device_context() const;
		ID3D11Debug* get_debug_interface() const noexcept { return m_debug_interface.Get(); }

		ID3D11RenderTargetView* get_default_render_target_view() const noexcept { return m_default_rtv.Get(); }
		ID3D11DepthStencilView* get_default_depth_stencil_view() const noexcept { return m_default_dsv.Get(); }

		ID3D11RenderTargetView** get_address_of_render_target_view() noexcept { return m_default_rtv.GetAddressOf(); }
		ID3D11DepthStencilView** get_address_of_depth_stencil_view() noexcept { return m_default_dsv.GetAddressOf(); }

		unsigned int get_MSAA_quality(int sample_count) const;
	};
}

#endif //GLACIER_ENGINE_D3D_CONTEXT_H_
