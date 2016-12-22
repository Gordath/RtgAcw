#include "D3D11_shader_program.h"
#include "D3D11_context.h"
#include "D3D11_shader.h"
#include "resource_manager.h"
#include "vertex.h"

namespace Glacier
{
	void D3D11ShaderProgram::create(const std::wstring& vs,
	                                const std::wstring& hs,
	                                const std::wstring& ds,
	                                const std::wstring& gs,
	                                const std::wstring& fs,
	                                unsigned int input_layout_mask) noexcept
	{
		D3D11Context* GAPI_context{ static_cast<D3D11Context*>(get_GAPI_context()) };

		ComPtr<ID3D11Device> device{ GAPI_context->get_device() };

		if (!vs.empty()) {
			m_shaders[VERTEX_SHADER] = ResourceManager::get<D3D11Shader>(vs);

			D3D11Shader* shader{ static_cast<D3D11Shader*>(m_shaders[VERTEX_SHADER]) };
			ComPtr<ID3DBlob> blob{ shader->get_blob() };

			HRESULT res{ 0 };
			res = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_vertex_shader.ReleaseAndGetAddressOf());
			if (FAILED(res)) {
				std::cerr << "Vertex shader creation failed! Aborting ShaderProgram creation." << std::endl;
				return;
			}

			std::vector<D3D11_INPUT_ELEMENT_DESC> input;

			if (input_layout_mask & IL_POSITION == 0) {
				input.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 });
			}

			if (input_layout_mask & IL_NORMAL == 0) {
				input.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0 });
			}

			if (input_layout_mask & IL_TANGENT == 0) {
				input.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, tangent), D3D11_INPUT_PER_VERTEX_DATA, 0 });
			}

			if (input_layout_mask & IL_TEXCOORD == 0) {
				input.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0 });
			}

			if (input_layout_mask & IL_COLOR == 0) {
				input.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 });
			}

			res = device->CreateInputLayout(input.data(), 5, blob->GetBufferPointer(), blob->GetBufferSize(), m_input_layout.ReleaseAndGetAddressOf());

			if (FAILED(res)) {
				std::cerr << "Input layout creation failed! Aborting ShaderProgram creation." << std::endl;
				return;
			}
		} else {
			std::cerr << "Failed to create shader program. Please provide at least a vertex shader file name!" << std::endl;
			return;
		}

		if (!hs.empty()) {
			m_shaders[HULL_SHADER] = ResourceManager::get<D3D11Shader>(hs);

			D3D11Shader* shader{ static_cast<D3D11Shader*>(m_shaders[DOMAIN_SHADER]) };

			ComPtr<ID3DBlob> blob{ shader->get_blob() };

			HRESULT res{ 0 };
			res = device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_hull_shader.ReleaseAndGetAddressOf());
			if (FAILED(res)) {
				std::cerr << "Hull shader creation failed!" << std::endl;
			}
			//TODO: get the blob and create the actual d3d shader.
		}

		if (!ds.empty()) {
			m_shaders[DOMAIN_SHADER] = ResourceManager::get<D3D11Shader>(ds);
			
			D3D11Shader* shader{ static_cast<D3D11Shader*>(m_shaders[DOMAIN_SHADER]) };

			ComPtr<ID3DBlob> blob{ shader->get_blob() };

			HRESULT res{ 0 };
			res = device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_domain_shader.ReleaseAndGetAddressOf());
			if (FAILED(res)) {
				std::cerr << "Domain shader creation failed!" << std::endl;
			}
		}

		if (!gs.empty()) {
			m_shaders[GEOMETRY_SHADER] = ResourceManager::get<D3D11Shader>(gs);

			D3D11Shader* shader{ static_cast<D3D11Shader*>(m_shaders[GEOMETRY_SHADER]) };

			ComPtr<ID3DBlob> blob{ shader->get_blob() };

			HRESULT res{ 0 };
			res = device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_geometry_shader.ReleaseAndGetAddressOf());
			if (FAILED(res)) {
				std::cerr << "Geometry shader creation failed!" << std::endl;
			}
		}

		if (!fs.empty()) {
			m_shaders[FRAGMENT_SHADER] = ResourceManager::get<D3D11Shader>(fs);
			
			D3D11Shader* shader{ static_cast<D3D11Shader*>(m_shaders[FRAGMENT_SHADER]) };
			
			ComPtr<ID3DBlob> blob{ shader->get_blob() };

			HRESULT res{ 0 };
			res = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pixel_shader.ReleaseAndGetAddressOf());
			if (FAILED(res)) {
				std::cerr << "Fragment shader creation failed!" << std::endl;
			}
		}
	}

	void D3D11ShaderProgram::bind() const noexcept
	{
		D3D11Context* context{ static_cast<D3D11Context*>(get_GAPI_context()) };

		ComPtr<ID3D11DeviceContext> device_context{ context->get_device_context() };

		device_context->VSSetShader(m_vertex_shader.Get(), nullptr, 0);
		device_context->IASetInputLayout(m_input_layout.Get());

		device_context->HSSetShader(m_hull_shader.Get(), nullptr, 0);

		device_context->DSSetShader(m_domain_shader.Get(), nullptr, 0);

		device_context->GSSetShader(m_geometry_shader.Get(), nullptr, 0);

		device_context->PSSetShader(m_pixel_shader.Get(), nullptr, 0);
	}
}
