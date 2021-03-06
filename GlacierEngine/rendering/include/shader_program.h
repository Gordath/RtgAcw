#ifndef GLACIER_SHADER_PROGRAM_H_
#define GLACIER_SHADER_PROGRAM_H_
#include "shader.h"
#include "../rendering/include/d3d/D3D11_shader.h"
#include <array>

namespace Glacier
{
	enum InputLayoutMask { /* parasoft-suppress  CODSTA-MCPP-03 "This enum is not scoped because it is used for bitwise OR operations for bitmasks." */
		IL_POSITION = 0x02,
		IL_NORMAL = 0x04,
		IL_TANGENT = 0x08,
		IL_TEXCOORD = 0x10,
		IL_COLOR = 0x20
	};

	static const std::wstring SHADER_PATH{ L"sdr\\" };

	class ShaderProgram {
	private:
#if defined(GLACIERENGINE_BUILD_D3D)
		std::array<D3D11Shader*, SHADER_COUNT> m_shaders;
#else
#endif
	public:
		ShaderProgram();

		virtual ~ShaderProgram();

		ShaderProgram(const ShaderProgram&) = default;

		ShaderProgram& operator=(const ShaderProgram&) = default;

#if defined(GLACIERENGINE_BUILD_D3D)
		const D3D11Shader* get_shader(ShaderType shader_type)
		{
			return m_shaders[shader_type];
		}

		void set_shader(D3D11Shader* shader, ShaderType shader_type) noexcept
		{
			m_shaders[shader_type] = shader;
		}
#else
#endif

		virtual bool create(unsigned int input_layout_mask,
		                    const std::wstring& vs,
		                    const std::wstring& fs = L"",
		                    const std::wstring& hs = L"",
		                    const std::wstring& ds = L"",
		                    const std::wstring& gs = L"") noexcept = 0;

		virtual void bind() const noexcept = 0;
	};
}

#endif //GLACIER_SHADER_PROGRAM_H_
