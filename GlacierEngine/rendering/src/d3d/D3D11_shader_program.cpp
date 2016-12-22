#include "D3D11_shader_program.h"
#include "D3D11_context.h"
#include "D3D11_shader.h"
#include "resource_manager.h"

namespace Glacier
{
	void D3D11ShaderProgram::create(const std::wstring& vs,
	                                const std::wstring& hs,
	                                const std::wstring& ds,
	                                const std::wstring& gs,
	                                const std::wstring& fs) noexcept
	{
		if (!vs.empty()) {
			m_shaders[VERTEX_SHADER] = ResourceManager::get<D3D11Shader>(vs);
			//TODO: get the blob and create the actual d3d shader.
		}

		if (!hs.empty()) {
			m_shaders[HULL_SHADER] = ResourceManager::get<D3D11Shader>(hs);
			//TODO: get the blob and create the actual d3d shader.
		}

		if (!ds.empty()) {
			m_shaders[DOMAIN_SHADER] = ResourceManager::get<D3D11Shader>(ds);
			//TODO: get the blob and create the actual d3d shader.
		}

		if (!gs.empty()) {
			m_shaders[GEOMETRY_SHADER] = ResourceManager::get<D3D11Shader>(gs);
			//TODO: get the blob and create the actual d3d shader.
		}

		if (!fs.empty()) {
			m_shaders[FRAGMENT_SHADER] = ResourceManager::get<D3D11Shader>(fs);
			//TODO: get the blob and create the actual d3d shader.
		}
	}

	void D3D11ShaderProgram::bind() const noexcept
	{
		D3D11Context* context{ static_cast<D3D11Context*>(get_GAPI_context()) };

		ComPtr<ID3D11DeviceContext> device_context{ context->get_device_context() };

		device_context->VSSetShader(m_vertex_shader.Get(), nullptr, 0);

		device_context->HSSetShader(m_hull_shader.Get(), nullptr, 0);

		device_context->DSSetShader(m_domain_shader.Get(), nullptr, 0);

		device_context->GSSetShader(m_geometry_shader.Get(), nullptr, 0);

		device_context->PSSetShader(m_pixel_shader.Get(), nullptr, 0);
	}
}
