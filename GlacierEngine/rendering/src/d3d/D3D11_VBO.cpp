#include "D3D11_VBO.h"
#include "D3D11_context.h"
#include <iostream>
#include "internal/engine_context.h"
#include <minwinbase.h>

namespace Glacier
{
	bool D3D11VBO::create(const std::vector<Vertex>& vertices, PrimitiveTopology primitive_topology) noexcept
	{
		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.ByteWidth = sizeof(Vertex) * vertices.size();
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


		D3D11Context* ctx{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11Device> device{ ctx->get_device() };
		HRESULT res = device->CreateBuffer(&buffer_desc, nullptr, m_vertex_buffer.ReleaseAndGetAddressOf());

		if (FAILED(res)) {
			std::cerr << "Failed to create the D3D11VBO." << std::endl;
			return false;
		}

		ComPtr<ID3D11DeviceContext> device_context{ ctx->get_device_context() };
		
		D3D11_MAPPED_SUBRESOURCE ms;
		device_context->Map(m_vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		memcpy(ms.pData, vertices.data(), sizeof(Vertex) * vertices.size());
		device_context->Unmap(m_vertex_buffer.Get(), 0);

		m_vertex_count = vertices.size();

		m_primitive_topology = primitive_topology;

		return true;
	}


	void D3D11VBO::bind() const noexcept
	{
		D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

		UINT stride{ sizeof(Vertex) };
		UINT offset{ 0 };
		device_context->IASetVertexBuffers(0, 1, m_vertex_buffer.GetAddressOf(), &stride, &offset);

		switch (m_primitive_topology) {
		case PrimitiveTopology::TRIANGLE_LIST:
			device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case PrimitiveTopology::TRIANGLE_STRIP:
			device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			break;
		default:
			break;
		}
		
	}

	void D3D11VBO::draw() const noexcept
	{
		D3D11Context* ctx{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> device_context{ ctx->get_device_context() };
		device_context->Draw(m_vertex_count, 0);
	}
}
