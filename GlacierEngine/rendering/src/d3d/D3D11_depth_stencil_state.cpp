#include "D3D11_depth_stencil_state.h"
#include <minwinbase.h>
#include "internal/engine_context.h"

namespace Glacier
{
	D3D11DepthStencilState::D3D11DepthStencilState(RenderStateType render_state_type)
	{
		switch (render_state_type) {
		case RenderStateType::DSS_DEPTH_TEST_DISABLE: {
			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.DepthEnable = false;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D11_COMPARISON_LESS;
			desc.StencilEnable = false;
			desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

			D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

			ComPtr<ID3D11Device> device{ GAPI_context->get_device() };

			HRESULT res = device->CreateDepthStencilState(&desc, m_depth_stencil_state.ReleaseAndGetAddressOf());
		}
			break;
		case RenderStateType::DSS_DEPTH_TEST_ENABLE: {
			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.DepthEnable = true;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D11_COMPARISON_LESS;
			desc.StencilEnable = false;
			desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

			D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

			ComPtr<ID3D11Device> device{ GAPI_context->get_device() };

			HRESULT res = device->CreateDepthStencilState(&desc, m_depth_stencil_state.ReleaseAndGetAddressOf());
		}
			break;
		case RenderStateType::DSS_DEPTH_MASK_0: {
			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.DepthEnable = true;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.DepthFunc = D3D11_COMPARISON_LESS;
			desc.StencilEnable = false;
			desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

			D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

			ComPtr<ID3D11Device> device{ GAPI_context->get_device() };

			HRESULT res = device->CreateDepthStencilState(&desc, m_depth_stencil_state.ReleaseAndGetAddressOf());
		}
			break;
		case RenderStateType::DSS_DEPTH_MASK_1: {
			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.DepthEnable = true;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D11_COMPARISON_LESS;
			desc.StencilEnable = false;
			desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

			D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

			ComPtr<ID3D11Device> device{ GAPI_context->get_device() };

			HRESULT res = device->CreateDepthStencilState(&desc, m_depth_stencil_state.ReleaseAndGetAddressOf());
		}
			break;
		default:
			break;
		}
	}

	void D3D11DepthStencilState::set() const noexcept
	{
		D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

		device_context->OMSetDepthStencilState(m_depth_stencil_state.Get(), 0);
	}
}
