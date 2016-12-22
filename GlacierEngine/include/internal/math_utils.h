#ifndef GLACIER_MATH_UTILS_H_
#define GLACIER_MATH_UTILS_H_
#include "types.h"

namespace Glacier
{
	namespace MathUtils
	{
		constexpr float PI_F{ glm::pi<float>() };

		constexpr double PI_D{ glm::pi<double>() };

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
