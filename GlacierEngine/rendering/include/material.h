#ifndef GLACIER_MATERIAL_H_
#define GLACIER_MATERIAL_H_
#include "texture.h"
#include "internal/types.h"
#include "render_state.h"
#include <array>

namespace Glacier
{
	struct Material {
		std::array<Texture*, SUPPORTED_TEX_COUNT> textures;
		Vec4f diffuse;
		Vec4f specular;
		Mat4f texture_matrix;
		std::string shader_program_name{ "sdrprog_default" };
		RenderStateType blend_state;

		Material();

		Material(const Material& other) = default;
		Material& operator=(const Material& other) = default;
	};
}

#endif //GLACIER_MATERIAL_H_
