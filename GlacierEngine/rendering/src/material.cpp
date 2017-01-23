#include "material.h"

namespace Glacier
{
	Material::Material() : blend_state{ RenderStateType::BS_BLEND_DISSABLED }
	{
		for (int i = 0; i < SUPPORTED_TEX_COUNT; ++i) {
			textures[i] = nullptr;
		}
	}
}