#include "D3D11_IBO.h"
#include "D3D11_context.h"
#include <iostream>

namespace Glacier
{
	bool D3D11IBO::create(const std::vector<unsigned int> &indices)
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

		D3D11Context* ctx{ dynamic_cast<D3D11Context*>(get_GAPI_context()) };
		if (!ctx) {
			std::cerr << "Failed to create the D3D11IBO: Graphics API context is not of type -> D3D11Context!" << std::endl;
			return false;
		}

		ComPtr<ID3D11Device> device{ ctx->get_device() };

		HRESULT res = device->CreateBuffer(&buffer_desc, &index_data, _index_buffer.ReleaseAndGetAddressOf());
		if (FAILED(res)) {
			std::cerr << "Failed to create the D3D11IBO." << std::endl;
			return false;
		}

		_index_count = indices.size();

		return true;
	}

	void D3D11IBO::draw() const
	{
		D3D11Context *ctx{ static_cast<D3D11Context*>(get_GAPI_context()) };

		ComPtr<ID3D11DeviceContext> device_context{ ctx->get_device_context() };

		device_context->DrawIndexed(_index_count, 0, 0);
	}
}