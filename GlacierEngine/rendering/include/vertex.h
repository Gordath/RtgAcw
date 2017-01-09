#ifndef GLACIER_ENGINE_VERTEX_H_
#define GLACIER_ENGINE_VERTEX_H_

#include "internal/types.h"

namespace Glacier
{
	struct Vertex {
		Vec4f position{ 0.0f, 0.0f, 0.0f, 1.0f};
		Vec4f normal;
		Vec4f tangent;
		Vec4f texcoord;
		Vec4f color;

		Vertex() = default;

		Vertex(const Vec4f& position,
		       const Vec4f& normal,
		       const Vec4f& tangent,
		       const Vec4f& texcoord,
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
