#ifndef GLACIER_MATH_UTILS_H_
#define GLACIER_MATH_UTILS_H_
#include "types.h"
#include <GLM/gtc/matrix_transform.inl>
#include <GLM/gtx/spline.hpp>

namespace Glacier
{
	namespace MathUtils
	{
		constexpr float PI_F{ glm::pi<float>() };

		constexpr double PI_D{ glm::pi<double>() };

		static Mat4f identity_matrix{ glm::mat4{ 1.0f } };

		inline Vec2f normalize(const Vec2f& vec)
		{
			return glm::normalize(vec);
		}

		inline Vec3f normalize(const Vec3f& vec)
		{
			return glm::normalize(vec);
		}

		inline Vec4f normalize(const Vec4f& vec)
		{
			return glm::normalize(vec);
		}

		inline float lengthf(const Vec2f& vec)
		{
			return glm::length(vec);
		}

		inline float lengthf(const Vec3f& vec)
		{
			return glm::length(vec);
		}

		inline float lengthf(const Vec4f& vec)
		{
			return glm::length(vec);
		}

		inline double lengthd(const Vec2d& vec)
		{
			return glm::length(vec);
		}

		inline double lengthd(const Vec3d& vec)
		{
			return glm::length(vec);
		}

		inline double lengthd(const Vec4d& vec)
		{
			return glm::length(vec);
		}

		inline Mat4f transpose(const Mat4f& mat)
		{
			return glm::transpose(mat);
		}

		inline Mat4f inverse(const Mat4f& mat)
		{
			return glm::inverse(mat);
		}

		inline Mat4f translate(const Mat4f& matrix, const Vec3f& vector)
		{
			return glm::translate(matrix, vector);
		}

		inline Mat4f rotate(const Mat4f& matrix, float angle, const Vec3f& axis)
		{
			return glm::rotate(matrix, angle, axis);
		}

		inline Mat4f scale(const Mat4f& matrix, const Vec3f& scale)
		{
			return glm::scale(matrix, scale);
		}

		inline Mat4f perspective_lh(float fov, float width, float height, float near_plane, float far_plane)
		{
			return glm::perspectiveFovLH(fov, width, height, near_plane, far_plane);
		}

		inline Vec3f spherical_to_cartesian(float theta, float phi, float radius = 1.0f)
		{
			return Vec3f{ radius * sin(theta) * sin(phi), radius * cos(phi), radius * cos(theta) * sin(phi) };
		}

		inline Mat4f lookat_lh(const Vec3f& position, const Vec3f& target, const Vec3f& up)
		{
			return glm::lookAtLH(position, target, up);
		}

		inline Vec3f catmull_rom(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Vec3f& d, float t)
		{
			return glm::catmullRom(a, b, c, d, t);
		}

		template<typename T>
		T to_radians(T degrees)
		{
			return glm::radians(degrees);
		}

		template<typename T>
		T to_degrees(T radians)
		{
			return glm::degrees(radians);
		}
	}
}

#endif //GLACIER_MATH_UTILS_H_
