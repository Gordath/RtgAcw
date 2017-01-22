#ifndef GLACIER_ENGINE_SHADER_H_
#define GLACIER_ENGINE_SHADER_H_
#include "resource.h"

namespace Glacier
{
	enum ShaderType { /* parasoft-suppress  CODSTA-MCPP-03 "This is not a scoped enum deliberatly. It is used as a most descriptive way to access specific indices in the array of shaders." */
		VERTEX_SHADER,
		HULL_SHADER,
		DOMAIN_SHADER,
		GEOMETRY_SHADER,
		FRAGMENT_SHADER,
		SHADER_COUNT
	};

	class Shader : public Resource {
	public:
		Shader() = default;
	};
}

#endif //GLACIER_ENGINE_SHADER_H_
