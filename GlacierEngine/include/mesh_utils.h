#ifndef GLACIER_MESH_UTILS_H_
#define GLACIER_MESH_UTILS_H_
#include "../rendering/include/mesh.h"

namespace Glacier
{
	namespace MeshUtils
	{
		Mesh* generate_cube(float size, VertexWinding vertex_winding = VertexWinding::CLOCKWISE) noexcept;

		Mesh* generate_uv_sphere(float radius, int resolution) noexcept;
	}
}

#endif //GLACIER_MESH_UTILS_H_
