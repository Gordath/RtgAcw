#include "D3D11_render_target.h"
#include "D3D11_context.h"
#include <iostream>

namespace Glacier
{
	bool D3D11RenderTarget::create()
	{
		D3D11Context *ctx{ dynamic_cast<D3D11Context*>(get_GAPI_context()) };
		
		if (!ctx) {
			std::cerr << "Cannot create D3D11RenderTarget: Graphics API context is not of type -> D3D11Context!" << std::endl;
			return false;
		}

		auto device{ ctx->get_device() };

		HRESULT res{ 0 };
		if (!_color_attachment) {
			D3D11_TEXTURE2D_DESC color_attachment_desc;
			color_attachment_desc.Width = _size.x;
			color_attachment_desc.Height = _size.y;
			color_attachment_desc.MipLevels = 1;
			color_attachment_desc.ArraySize = 1;
			color_attachment_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

			if (_MSAA) {
				color_attachment_desc.SampleDesc.Count = _sample_count;
				color_attachment_desc.SampleDesc.Quality = ctx->get_MSAA_quality(_sample_count) - 1;
			}
			else {
				color_attachment_desc.SampleDesc.Count = 1;
				color_attachment_desc.SampleDesc.Quality = 0;
			}

			color_attachment_desc.Usage = D3D11_USAGE_DEFAULT;
			color_attachment_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			color_attachment_desc.CPUAccessFlags = 0;
			color_attachment_desc.MiscFlags = 0;

			res = device->CreateTexture2D(&color_attachment_desc, nullptr, _color_attachment.ReleaseAndGetAddressOf());

			if (FAILED(res)) {
				std::cerr << "Failed to Create the D3D11RenderTarget color attachment!" << std::endl;
				return false;
			}
		}

		res = device->CreateRenderTargetView(_color_attachment.Get(), nullptr, _render_target_view.ReleaseAndGetAddressOf());
		if(FAILED(res)) {
			std::cerr << "Failed to create the D3D11RenderTarget render target view!" << std::endl;
			return false;
		}


		D3D11_TEXTURE2D_DESC depth_attachment_desc;
		depth_attachment_desc.Width = _size.x;
		depth_attachment_desc.Height = _size.y;
		depth_attachment_desc.MipLevels = 1;
		depth_attachment_desc.ArraySize = 1;
		depth_attachment_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		if (_MSAA) {
			depth_attachment_desc.SampleDesc.Count = _sample_count;
			depth_attachment_desc.SampleDesc.Quality = ctx->get_MSAA_quality(_sample_count) - 1;
		}
		else {
			depth_attachment_desc.SampleDesc.Count = 1;
			depth_attachment_desc.SampleDesc.Quality = 0;
		}

		depth_attachment_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_attachment_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_attachment_desc.CPUAccessFlags = 0;
		depth_attachment_desc.MiscFlags = 0;

		res = device->CreateTexture2D(&depth_attachment_desc, nullptr, _depth_attachment.ReleaseAndGetAddressOf());

		if (FAILED(res)) {
			std::cerr << "Failed to create D3D11RenderTarget depth attachment!" << std::endl;
			return false;
		}

		res = device->CreateDepthStencilView(_depth_attachment.Get(), nullptr, _depth_buffer_view.ReleaseAndGetAddressOf());

		if (FAILED(res)) {
			std::cerr << "Failed to create D3D11RenderTarget depth stencil view!" << std::endl;
			return false;
		}

		return true;
	}

	bool D3D11RenderTarget::bind() const
	{
		D3D11Context *ctx{ dynamic_cast<D3D11Context*>(get_GAPI_context())};

		if (!ctx) {
			std::cerr << "Cannot bind D3D11RenderTarget: Graphics API context is not of type -> D3D11Context!" << std::endl;
			return false;
		}

		ComPtr<ID3D11DeviceContext> context{ ctx->get_device_context() };
		context->OMSetRenderTargets(1, _render_target_view.GetAddressOf(), _depth_buffer_view.Get());

		return true;
	}

	bool D3D11RenderTarget::unbind() const
	{
		D3D11Context *ctx{ dynamic_cast<D3D11Context*>(get_GAPI_context()) };

		if (!ctx) {
			std::cerr << "Cannot bind D3D11RenderTarget: Graphics API context is not of type -> D3D11Context!" << std::endl;
			return false;
		}

		ComPtr<ID3D11DeviceContext> context{ ctx->get_device_context() };
		context->OMSetRenderTargets(1, nullptr, nullptr);

		return true;
	}

}

