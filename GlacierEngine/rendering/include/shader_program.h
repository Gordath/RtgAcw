#ifndef GLACIER_SHADER_PROGRAM_H_
#define GLACIER_SHADER_PROGRAM_H_
#include "shader.h"

namespace Glacier
{
	class ShaderProgram {
	protected:
		Shader* m_shaders[SHADER_COUNT];

	public:
		virtual ~ShaderProgram() = default;

		virtual void create(const std::wstring& vs,
		                    const std::wstring& hs,
		                    const std::wstring& ds,
		                    const std::wstring& gs,
		                    const std::wstring& fs) noexcept = 0;

		virtual void bind() const noexcept = 0;
	};
}

#endif //GLACIER_SHADER_PROGRAM_H_
