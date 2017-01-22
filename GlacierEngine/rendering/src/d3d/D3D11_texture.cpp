#include "D3D11_texture.h"
#include <D3D/D3DX11tex.h>
#include <iostream>
#include "D3D11_context.h"
#include "internal/engine_context.h"

namespace Glacier
{
	bool D3D11_texture::load(const std::wstring& file_name) noexcept
	{
		D3D11Context* context{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11Device> device{ context->get_device() };

		HRESULT res{ D3DX11CreateShaderResourceViewFromFile(device.Get(),
													 file_name.c_str(),
													 nullptr,
													 nullptr,
													 m_shader_resource_view.ReleaseAndGetAddressOf(),
													 nullptr) };

		if (FAILED(res)) {
			std::wcerr << L"Failed to create texture from file " + file_name + L"! Shader resource view creation failed." << std::endl;
			return false;
		}

		return true;
	}

	void D3D11_texture::bind() const noexcept
	{
		D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

		device_context->PSSetShaderResources(get_texture_type(), 1, m_shader_resource_view.GetAddressOf());
	}
}
