#ifndef GLACIER_MATERIAL_H_
#define GLACIER_MATERIAL_H_
#include "texture.h"
#include "internal/types.h"

namespace Glacier
{
	struct Material {
		Texture* textures[SUPPORTED_TEX_COUNT];
		Vec4f diffuse;
		Vec4f specular;
		float shininess;
	};
}

#endif //GLACIER_MATERIAL_H_
