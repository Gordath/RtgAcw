#ifndef GLACIER_ENGINE_SHADER_H_
#define GLACIER_ENGINE_SHADER_H_
#include "resource.h"

namespace Glacier
{
	enum ShaderType {
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
