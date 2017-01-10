#include "D3D11_render_target.h"
#include "D3D11_context.h"
#include <iostream>
#include "internal/engine_context.h"

namespace Glacier
{
	bool D3D11RenderTarget::create(const Vec2i& size)
	{
		m_size = size;

		D3D11Context* ctx{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11Device> device{ ctx->get_device() };

		HRESULT res{ 0 };

		D3D11_TEXTURE2D_DESC color_attachment_desc;
		color_attachment_desc.Width = m_size.x;
		color_attachment_desc.Height = m_size.y;
		color_attachment_desc.MipLevels = 1;
		color_attachment_desc.ArraySize = 1;
		color_attachment_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		if (m_MSAA) {
			color_attachment_desc.SampleDesc.Count = m_sample_count;
			color_attachment_desc.SampleDesc.Quality = ctx->get_MSAA_quality(m_sample_count) - 1;
		}
		else {
			color_attachment_desc.SampleDesc.Count = 1;
			color_attachment_desc.SampleDesc.Quality = 0;
		}

		color_attachment_desc.Usage = D3D11_USAGE_DEFAULT;
		color_attachment_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		color_attachment_desc.CPUAccessFlags = 0;
		color_attachment_desc.MiscFlags = 0;

		res = device->CreateTexture2D(&color_attachment_desc, nullptr, m_color_attachment.ReleaseAndGetAddressOf());

		if (FAILED(res)) {
			std::cerr << "Failed to Create the D3D11RenderTarget color attachment!" << std::endl;
			return false;
		}

		D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
		rtv_desc.Format = color_attachment_desc.Format;
		rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtv_desc.Texture2D.MipSlice = 0;

		res = device->CreateRenderTargetView(m_color_attachment.Get(), &rtv_desc, m_render_target_view.ReleaseAndGetAddressOf());
		if (FAILED(res)) {
			std::cerr << "Failed to create the D3D11RenderTarget render target view!" << std::endl;
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		srv_desc.Format = color_attachment_desc.Format;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_desc.Texture2D.MostDetailedMip = 0;
		srv_desc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		res = device->CreateShaderResourceView(m_color_attachment.Get(), &srv_desc, m_color_attachment_srv.ReleaseAndGetAddressOf());
		if (FAILED(res)) {
			std::cerr << "Failed to create the color attachment shader resource view!" << std::endl;
			return false;
		}

		D3D11_TEXTURE2D_DESC depth_attachment_desc;
		depth_attachment_desc.Width = m_size.x;
		depth_attachment_desc.Height = m_size.y;
		depth_attachment_desc.MipLevels = 1;
		depth_attachment_desc.ArraySize = 1;
		depth_attachment_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		if (m_MSAA) {
			depth_attachment_desc.SampleDesc.Count = m_sample_count;
			depth_attachment_desc.SampleDesc.Quality = ctx->get_MSAA_quality(m_sample_count) - 1;
		}
		else {
			depth_attachment_desc.SampleDesc.Count = 1;
			depth_attachment_desc.SampleDesc.Quality = 0;
		}

		depth_attachment_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_attachment_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_attachment_desc.CPUAccessFlags = 0;
		depth_attachment_desc.MiscFlags = 0;

		res = device->CreateTexture2D(&depth_attachment_desc, nullptr, m_depth_attachment.ReleaseAndGetAddressOf());

		if (FAILED(res)) {
			std::cerr << "Failed to create D3D11RenderTarget depth attachment!" << std::endl;
			return false;
		}

		res = device->CreateDepthStencilView(m_depth_attachment.Get(), nullptr, m_depth_buffer_view.ReleaseAndGetAddressOf());

		if (FAILED(res)) {
			std::cerr << "Failed to create D3D11RenderTarget depth stencil view!" << std::endl;
			return false;
		}

		return true;
	}

	bool D3D11RenderTarget::bind() const
	{
		D3D11Context* ctx{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> context{ ctx->get_device_context() };
		context->OMSetRenderTargets(1, m_render_target_view.GetAddressOf(), m_depth_buffer_view.Get());

		return true;
	}

	bool D3D11RenderTarget::unbind() const
	{
		D3D11Context* ctx{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> context{ ctx->get_device_context() };
		context->OMSetRenderTargets(0, nullptr, nullptr);

		return true;
	}

	void D3D11RenderTarget::clear(float* color) const noexcept
	{
		D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

		device_context->ClearRenderTargetView(m_render_target_view.Get(), color);
		device_context->ClearDepthStencilView(m_depth_buffer_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

}
