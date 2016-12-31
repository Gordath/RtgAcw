#ifndef GLACIER_TYPES_H_
#define GLACIER_TYPES_H_

#include "GLM/glm.hpp"
#include "GLM/gtc/constants.inl"
#include "wrl.h"

namespace Glacier
{
	using Vec2i = glm::ivec2;
	using Vec3i = glm::ivec3;
	using Vec4i = glm::ivec4;

	using Vec2f = glm::vec2;
	using Vec3f = glm::vec3;
	using Vec4f = glm::vec4;

	using Vec2d = glm::dvec2;
	using Vec3d = glm::dvec3;
	using Vec4d = glm::dvec4;

	using Mat2f = glm::mat2;
	using Mat3f = glm::mat3;
	using Mat4f = glm::mat4;

	using Mat2d = glm::dmat2;
	using Mat3d = glm::dmat3;
	using Mat4d = glm::dmat4;

	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
}


#endif //GLACIER_TYPES_H_
