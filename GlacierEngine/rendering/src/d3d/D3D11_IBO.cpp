#include "D3D11_IBO.h"
#include "D3D11_context.h"
#include <iostream>
#include "internal/engine_context.h"
#include <minwinbase.h>

namespace Glacier
{
	bool D3D11IBO::create(const std::vector<unsigned int>& indices) noexcept
	{
		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.ByteWidth = sizeof(unsigned int) * indices.size();
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11Context* ctx{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11Device> device{ ctx->get_device() };

		HRESULT res = device->CreateBuffer(&buffer_desc, nullptr, m_index_buffer.ReleaseAndGetAddressOf());
		if (FAILED(res)) {
			std::cerr << "Failed to create the D3D11IBO." << std::endl;
			return false;
		}

		D3D11_MAPPED_SUBRESOURCE index_data;
		ZeroMemory(&index_data, sizeof(index_data));

		ComPtr<ID3D11DeviceContext> device_context{ ctx->get_device_context() };
		
		device_context->Map(m_index_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &index_data);
		memcpy(index_data.pData, indices.data(), sizeof(unsigned int) * indices.size());
		device_context->Unmap(m_index_buffer.Get(), 0);

		set_index_count(indices.size());

		return true;
	}

	void D3D11IBO::bind() const noexcept
	{
		D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

		device_context->IASetIndexBuffer(m_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void D3D11IBO::draw() const noexcept
	{
		D3D11Context* ctx{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> device_context{ ctx->get_device_context() };

		device_context->DrawIndexed(get_index_count(), 0, 0);
	}
}
