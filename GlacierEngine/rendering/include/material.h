#ifndef GLACIER_MATERIAL_H_
#define GLACIER_MATERIAL_H_
#include "texture.h"
#include "internal/types.h"
#include "render_state.h"

namespace Glacier
{
	struct Material {
		Texture* textures[SUPPORTED_TEX_COUNT];
		Vec4f diffuse;
		Vec4f specular;
		Mat4f texture_matrix;
		std::string shader_program_name{ "sdrprog_default" };
		RenderStateType blend_state;

		Material() : blend_state{ RenderStateType::BS_BLEND_DISSABLED }
		{
			for (int i = 0; i < SUPPORTED_TEX_COUNT; ++i) {
				textures[i] = nullptr;
			}
		}
	};
}

#endif //GLACIER_MATERIAL_H_
