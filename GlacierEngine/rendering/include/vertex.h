#ifndef GLACIER_ENGINE_VERTEX_H_
#define GLACIER_ENGINE_VERTEX_H_

#include "internal/types.h"

namespace Glacier
{
	struct Vertex {
		Vec3f position;
		Vec3f normal;
		Vec3f tangent;
		Vec2f texcoord;
		Vec4f color;

		Vertex() = default;

		Vertex(const Vec3f& position,
		       const Vec3f& normal,
		       const Vec3f& tangent,
		       const Vec2f& texcoord,
		       const Vec4f& color) : position{ position },
		                             normal{ normal },
		                             tangent{ tangent },
		                             texcoord{ texcoord },
		                             color{ color }
		{
		}
	};
}

#endif //GLACIER_ENGINE_VERTEX_H_
