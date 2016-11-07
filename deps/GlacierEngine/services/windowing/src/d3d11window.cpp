#include "d3d11window.h"
#include <array>
#include <assert.h>


namespace Glacier
{
	ComPtr<ID3D11Device> D3D11Window::_device;
	ComPtr<ID3D11DeviceContext> D3D11Window::_context;

	bool D3D11Window::create_D3D11_device(D3D11Window* window)
	{
		unsigned int device_flags{0};

#if defined(DEBUG) || defined(_DEBUG)
		device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		std::array<D3D_FEATURE_LEVEL, 3> feature_levels = {
			{D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0}
		};

		D3D_FEATURE_LEVEL feature_level;

		if (!_device) {
			HRESULT h_result{ 0 };
			h_result = D3D11CreateDevice(nullptr,
			                             D3D_DRIVER_TYPE_HARDWARE,
			                             nullptr,
			                             device_flags,
			                             &feature_levels[0],
			                             feature_levels.size(),
			                             D3D11_SDK_VERSION,
			                             _device.GetAddressOf(),
			                             &feature_level,
			                             _context.GetAddressOf());

			LPCSTR err_msg;
			if (FAILED(h_result)) {
				err_msg = "D3D11CreateDevice Failed.";
				MessageBox(window->_handle, err_msg, nullptr, 0);
				return false;
			}

			if (feature_level != D3D_FEATURE_LEVEL_11_0) {
				err_msg = "Direct3D Feature Level 11 unsupported.";
				MessageBox(window->_handle, err_msg, nullptr, 0);
			}
		}


		return true;
	}

	bool D3D11Window::create_D3D11_swap_chain(D3D11Window* window)
	{
		//Describe the swapchain
		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		Vec2i win_size{window->get_size()};
		swap_chain_desc.BufferDesc.Width = win_size.x;
		swap_chain_desc.BufferDesc.Height = win_size.y;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		if (window->MSAA_enabled()) {
			swap_chain_desc.SampleDesc.Count = window->get_sample_count();
			swap_chain_desc.SampleDesc.Quality = window->get_MSAA_quality() - 1;
		}
		else { //No MSAA
			swap_chain_desc.SampleDesc.Count = 0;
			swap_chain_desc.SampleDesc.Quality = 0;
		}

		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.BufferCount = 1; //1 back buffer = Double buffering
		swap_chain_desc.OutputWindow = window->_handle;
		swap_chain_desc.Windowed = true; //TODO: Ckeck if this is the fullscreen switch.
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swap_chain_desc.Flags = 0;

		HRESULT h_result{0};

		ComPtr<IDXGIDevice> dxgi_device{nullptr};
		h_result = _device.As(&dxgi_device);

		ComPtr<IDXGIAdapter> dxgi_adapter{nullptr};
		h_result = dxgi_device->GetAdapter(dxgi_adapter.GetAddressOf());

		//Now get the factory interface that was used to create the _device.
		ComPtr<IDXGIFactory> dxgi_factory{nullptr};
		h_result = dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)dxgi_factory.GetAddressOf());

		//At last create the swap chain.
		h_result = dxgi_factory->CreateSwapChain(_device.Get(), &swap_chain_desc, window->_swap_chain.GetAddressOf());

		if (FAILED(h_result)) {
			LPCSTR err_msg{"DXGISwapChain creation failed."};
			MessageBox(window->_handle, err_msg, nullptr, 0);
			return false;
		}

		return true;
	}

	unsigned int D3D11Window::check_MSAA_quality(D3D11Window* window)
	{
		unsigned int quality;
		_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
		                                       window->get_sample_count(),
		                                       &quality);
		assert(quality > 0);

		return quality;
	}

	bool D3D11Window::init_D3D(D3D11Window* window)
	{
		if (!create_D3D11_device(window)) {
			return false;
		}

		//Check MSAA Quality level
		window->_MSAA_quality = check_MSAA_quality(window);

		if (!create_D3D11_swap_chain(window)) {
			return false;
		}

		return true;
	}

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
