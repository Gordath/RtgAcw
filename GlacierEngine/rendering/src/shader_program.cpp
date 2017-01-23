#include "shader_program.h"

namespace Glacier
{

	ShaderProgram::ShaderProgram()
	{
		for (int i = 0; i < SHADER_COUNT; ++i) {
			m_shaders[i] = nullptr;
		}
	}

	ShaderProgram::~ShaderProgram()
	{
	}
}
