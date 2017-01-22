#ifndef GLACIER_SHADER_PROGRAM_H_
#define GLACIER_SHADER_PROGRAM_H_
#include "shader.h"
#include "../rendering/include/d3d/D3D11_shader.h"

namespace Glacier
{
	enum InputLayoutMask {
		IL_POSITION = 0x02,
		IL_NORMAL = 0x04,
		IL_TANGENT = 0x08,
		IL_TEXCOORD = 0x10,
		IL_COLOR = 0x20
	};

	static const std::wstring SHADER_PATH{ L"sdr\\" };

	class ShaderProgram {
	protected:
#if defined(GLACIERENGINE_BUILD_D3D)
		D3D11Shader* m_shaders[SHADER_COUNT];
#else
#endif
	public:
		ShaderProgram()
		{
			for (int i = 0; i < SHADER_COUNT; ++i) {
				m_shaders[i] = nullptr;
			}
		}

		virtual ~ShaderProgram() = default;

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
