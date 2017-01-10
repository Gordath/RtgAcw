#ifndef GLACIER_SHADER_PROGRAM_H_
#define GLACIER_SHADER_PROGRAM_H_
#include "shader.h"

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
		Shader* m_shaders[SHADER_COUNT];

	public:
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
