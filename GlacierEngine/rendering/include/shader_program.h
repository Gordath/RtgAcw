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

	class ShaderProgram {
	protected:
		Shader* m_shaders[SHADER_COUNT];

	public:
		virtual ~ShaderProgram() = default;

		virtual void create(const std::wstring& vs,
		                    const std::wstring& hs,
		                    const std::wstring& ds,
		                    const std::wstring& gs,
		                    const std::wstring& fs,
		                    unsigned int input_layout_mask) noexcept = 0;

		virtual void bind() const noexcept = 0;
	};
}

#endif //GLACIER_SHADER_PROGRAM_H_
