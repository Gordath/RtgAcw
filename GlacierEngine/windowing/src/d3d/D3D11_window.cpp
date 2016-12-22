#include "D3D11_window.h"
#include <assert.h>
#include "D3D11_context.h"
#include <iostream>


namespace Glacier
{
	// Private Methods -----------------------------------------------------------------------------------------------------------------------------
	bool D3D11Window::create_swap_chain(const D3D11Context* ctx)
	{
		//Describe the swapchain
		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		swap_chain_desc.BufferDesc.Width = m_size.x;
		swap_chain_desc.BufferDesc.Height = m_size.y;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		if (m_enable_MSAA) {
			swap_chain_desc.SampleDesc.Count = m_sample_count;
			swap_chain_desc.SampleDesc.Quality = ctx->get_MSAA_quality(m_sample_count) - 1;
		}
		else { //No MSAA
			swap_chain_desc.SampleDesc.Count = 0;
			swap_chain_desc.SampleDesc.Quality = 0;
		}

		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.BufferCount = 1; //1 back buffer == Double buffering
		swap_chain_desc.OutputWindow = m_handle;
		swap_chain_desc.Windowed = true; //TODO: Ckeck if this is the fullscreen switch.
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swap_chain_desc.Flags = 0;

		HRESULT h_result{ 0 };

		ComPtr<ID3D11Device> device{ ctx->get_device() };

		ComPtr<IDXGIDevice> dxgi_device{ nullptr };
		h_result = device.As(&dxgi_device);

		ComPtr<IDXGIAdapter> dxgi_adapter{ nullptr };
		h_result = dxgi_device->GetAdapter(dxgi_adapter.GetAddressOf());

		//Now get the factory interface that was used to create the m_device.
		ComPtr<IDXGIFactory> dxgi_factory{ nullptr };
		h_result = dxgi_adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgi_factory.GetAddressOf()));

		//At last create the swap chain.
		h_result = dxgi_factory->CreateSwapChain(device.Get(), &swap_chain_desc, m_swap_chain.ReleaseAndGetAddressOf());

		if (FAILED(h_result)) {
			MessageBox(m_handle, L"DXGISwapChain creation failed.", nullptr, 0);
			return false;
		}

		return true;
	}

	bool D3D11Window::create_render_target_view(const D3D11Context* ctx) noexcept
	{
		HRESULT res{ 0 };

		// get the address of the back buffer
		ComPtr<ID3D11Texture2D> backbuffer;
		res = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backbuffer.GetAddressOf()));

		if (FAILED(res)) {
			std::cerr << "Failed to get the adress of the back buffer." << std::endl;
			return false;
		}
		
		// use the back buffer address to create the render target
		ComPtr<ID3D11Device> device{ ctx->get_device() };
		res = device->CreateRenderTargetView(backbuffer.Get(), nullptr, m_render_target.ReleaseAndGetAddressOf());

		if (FAILED(res)) {
			std::cerr << "Window render target view creation failed." << std::endl;
			return false;
		}

		return true;
	}

	bool D3D11Window::create_depth_stencil_view(const D3D11Context* ctx) noexcept
	{
		D3D11_TEXTURE2D_DESC depth_attachment_desc;
		ZeroMemory(&depth_attachment_desc, sizeof(depth_attachment_desc));
		depth_attachment_desc.Width = m_size.x;
		depth_attachment_desc.Height = m_size.y;
		depth_attachment_desc.MipLevels = 1;
		depth_attachment_desc.ArraySize = 1;
		depth_attachment_desc.Format = DXGI_FORMAT_D32_FLOAT;
		depth_attachment_desc.SampleDesc.Count = m_sample_count;
		depth_attachment_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		HRESULT res{ 0 };
		ComPtr<ID3D11Texture2D> depth;
		ComPtr<ID3D11Device> device{ ctx->get_device() };
		res = device->CreateTexture2D(&depth_attachment_desc, nullptr, depth.GetAddressOf());

		if (FAILED(res)) {
			std::cerr << "Failed to create the depth stencil view texture." << std::endl;
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

		res = device->CreateDepthStencilView(depth.Get(), &dsvd, m_depth_stencil.ReleaseAndGetAddressOf());

		if (FAILED(res)) {
			std::cerr << "Window depth stencil view creation failed." << std::endl;
			return false;
		}

		return true;
	}

	bool D3D11Window::initialize()
	{
		D3D11Context* ctx = dynamic_cast<D3D11Context*>(get_GAPI_context());
		if (!ctx) {
			std::cerr << "Cannot create D3D11RenderTargetView: Graphics API context is not of type -> D3D11Context!" << std::endl;
			return false;
		}

		if (!create_swap_chain(ctx)) {
			return false;
		}

		if (!create_render_target_view(ctx)) {
			return false;
		}

		if (!create_depth_stencil_view(ctx)) {
			return false;
		}

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<float>(m_size.x);
		viewport.Height = static_cast<float>(m_size.y);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		ComPtr<ID3D11DeviceContext> context{ ctx->get_device_context() };
		context->RSSetViewports(1, &viewport);

		return true;
	}

	// --------------------------------------------------------------------------------------------------------------------------------------------
}
