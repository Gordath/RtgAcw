#include "D3D11_VBO.h"
#include "D3D11_context.h"
#include <iostream>
#include "internal/engine_context.h"

namespace Glacier
{
	bool D3D11VBO::create(const std::vector<Vertex>& vertices)
	{
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.ByteWidth = sizeof(Vertex) * vertices.size();
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertex_data;
		vertex_data.pSysMem = vertices.data();
		vertex_data.SysMemPitch = 0;
		vertex_data.SysMemSlicePitch = 0;

		D3D11Context* ctx{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11Device> device{ ctx->get_device() };
		HRESULT res = device->CreateBuffer(&buffer_desc, &vertex_data, m_vertex_buffer.ReleaseAndGetAddressOf());

		if (FAILED(res)) {
			std::cerr << "Failed to create the D3D11VBO." << std::endl;
			return false;
		}

		m_vertex_count = vertices.size();

		return true;
	}

	void D3D11VBO::draw() const
	{
		D3D11Context* ctx{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> device_context{ ctx->get_device_context() };
		device_context->Draw(m_vertex_count, 0);
	}
}
