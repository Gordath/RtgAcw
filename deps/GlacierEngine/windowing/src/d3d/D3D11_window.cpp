#include "D3D11_window.h"
#include <assert.h>
#include "D3D11_context.h"
#include <iostream>


namespace Glacier
{
	// Private Methods -----------------------------------------------------------------------------------------------------------------------------
	bool D3D11Window::create_D3D11_swap_chain(const D3D11Context* ctx)
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

		if (_enable_MSAA) {
			swap_chain_desc.SampleDesc.Count = _sample_count;
			swap_chain_desc.SampleDesc.Quality = ctx->get_MSAA_quality(_sample_count) - 1;
		}
		else { //No MSAA
			swap_chain_desc.SampleDesc.Count = 0;
			swap_chain_desc.SampleDesc.Quality = 0;
		}

		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.BufferCount = 1; //1 back buffer == Double buffering
		swap_chain_desc.OutputWindow = _handle;
		swap_chain_desc.Windowed = true; //TODO: Ckeck if this is the fullscreen switch.
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swap_chain_desc.Flags = 0;

		HRESULT h_result{ 0 };

		ComPtr<ID3D11Device> device{ ctx->get_device() };

		ComPtr<IDXGIDevice> dxgi_device{ nullptr };
		h_result = device.As(&dxgi_device);

		ComPtr<IDXGIAdapter> dxgi_adapter{ nullptr };
		h_result = dxgi_device->GetAdapter(dxgi_adapter.GetAddressOf());

		//Now get the factory interface that was used to create the _device.
		ComPtr<IDXGIFactory> dxgi_factory{ nullptr };
		h_result = dxgi_adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgi_factory.GetAddressOf()));

		//At last create the swap chain.
		h_result = dxgi_factory->CreateSwapChain(device.Get(), &swap_chain_desc, _swap_chain.ReleaseAndGetAddressOf());

		if (FAILED(h_result)) {
			LPCSTR err_msg{ "DXGISwapChain creation failed." };
			MessageBox(_handle, err_msg, nullptr, 0);
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

		if (!create_D3D11_swap_chain(ctx)) {
			return false;
		}

		ID3D11Texture2D* backbuffer;
		_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer));
		_render_target = std::make_unique<D3D11RenderTarget>(m_size, _enable_MSAA, _sample_count, backbuffer);
		
		if (!_render_target->create()) {
			return false;
		}

		_render_target->bind();

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

	void D3D11Window::enable_MSAA(bool state)
	{
		_enable_MSAA = state;
	}

	bool D3D11Window::MSAA_enabled() const
	{
		return _enable_MSAA;
	}

	int D3D11Window::get_sample_count() const
	{
		return _sample_count;
	}

	unsigned int D3D11Window::get_MSAA_quality() const
	{
		return _MSAA_quality;
	}
}
