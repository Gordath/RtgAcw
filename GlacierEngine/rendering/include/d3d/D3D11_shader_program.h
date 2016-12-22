#ifndef GLACIER_D3D11_SHADER_PROGRAM_H_
#define GLACIER_D3D11_SHADER_PROGRAM_H_
#include "shader_program.h"
#include "GAPI_context_locator.h"
#include "internal/types.h"
#include "D3D11_IBO.h"

namespace Glacier
{
	class D3D11ShaderProgram : public ShaderProgram,
	                           protected GAPIContextLocator {
	private:
		ComPtr<ID3D11VertexShader> m_vertex_shader;
		ComPtr<ID3D11HullShader> m_hull_shader;
		ComPtr<ID3D11DomainShader> m_domain_shader;
		ComPtr<ID3D11GeometryShader> m_geometry_shader;
		ComPtr<ID3D11PixelShader> m_pixel_shader;

	public:
		void create(const std::wstring& vs,
		            const std::wstring& hs,
		            const std::wstring& ds,
		            const std::wstring& gs,
		            const std::wstring& fs) noexcept override;

		void bind() const noexcept override;
	};
}

#endif //GLACIER_D3D11_SHADER_PROGRAM_H_
