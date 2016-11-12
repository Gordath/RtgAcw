#ifndef GLACIER_ENGINE_VERTEX_H_
#define GLACIER_ENGINE_VERTEX_H_

#include "types.h"

namespace Glacier
{
	struct Vertex {
		Vec3f position;
		Vec3f normal;
		Vec3f tangent;
		Vec4f color;
	};
}

#endif //GLACIER_ENGINE_VERTEX_H_