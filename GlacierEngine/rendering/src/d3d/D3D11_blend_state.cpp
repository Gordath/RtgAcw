#include "D3D11_blend_state.h"
#include "D3D11_context.h"
#include "internal/engine_context.h"

namespace Glacier
{
	D3D11BlendState::D3D11BlendState(RenderStateType render_state_type)
	{
		switch (render_state_type) {
		case RenderStateType::BS_BLEND_DISSABLED: {
			D3D11_BLEND_DESC blend_desc;
			ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
			blend_desc.RenderTarget[0].BlendEnable = false;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

			ComPtr<ID3D11Device> device{ GAPI_context->get_device() };

			HRESULT res = device->CreateBlendState(&blend_desc, m_blend_state.ReleaseAndGetAddressOf());
		}
			break;
		case RenderStateType::BS_BLEND_ADDITIVE: {
			D3D11_BLEND_DESC blend_desc;
			ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
			blend_desc.RenderTarget[0].BlendEnable = true;
			blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

			ComPtr<ID3D11Device> device{ GAPI_context->get_device() };

			HRESULT res = device->CreateBlendState(&blend_desc, m_blend_state.ReleaseAndGetAddressOf());
		}
			break;
		case RenderStateType::BS_BLEND_ALPHA: {
			D3D11_BLEND_DESC blend_desc;
			ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
			blend_desc.RenderTarget[0].BlendEnable = true;
			blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

			ComPtr<ID3D11Device> device{ GAPI_context->get_device() };

			HRESULT res = device->CreateBlendState(&blend_desc, m_blend_state.ReleaseAndGetAddressOf());
		}
			break;
		default:
			break;
		}
	}

	void D3D11BlendState::set() const noexcept
	{
		D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

		device_context->OMSetBlendState(m_blend_state.Get(), nullptr, 0xffffffff);
	}
}
