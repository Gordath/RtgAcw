#include "D3D11_IBO.h"
#include "D3D11_context.h"
#include <iostream>
#include "internal/engine_context.h"

namespace Glacier
{
	bool D3D11IBO::create(const std::vector<unsigned int>& indices)
	{
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.ByteWidth = sizeof(unsigned int) * indices.size();
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA index_data;
		index_data.pSysMem = indices.data();
		index_data.SysMemPitch = 0;
		index_data.SysMemSlicePitch = 0;

		D3D11Context* ctx{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11Device> device{ ctx->get_device() };

		HRESULT res = device->CreateBuffer(&buffer_desc, &index_data, m_index_buffer.ReleaseAndGetAddressOf());
		if (FAILED(res)) {
			std::cerr << "Failed to create the D3D11IBO." << std::endl;
			return false;
		}

		m_index_count = indices.size();

		return true;
	}

	void D3D11IBO::draw() const
	{
		D3D11Context* ctx{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> device_context{ ctx->get_device_context() };

		device_context->DrawIndexed(m_index_count, 0, 0);
	}
}
