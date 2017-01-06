#ifndef GLACIER_MESH_UTILS_H_
#define GLACIER_MESH_UTILS_H_
#include "../rendering/include/mesh.h"

namespace Glacier
{
	namespace MeshUtils
	{
		Mesh* generate_cube(float size, VertexWinding vertex_winding = VertexWinding::CLOCKWISE) noexcept;

		Mesh* generate_uv_sphere(float radius,
		                         int slices,
		                         int stacks,
		                         float u_range,
		                         float v_range,
		                         VertexWinding vertex_winding = VertexWinding::CLOCKWISE) noexcept;
	}
}

#endif //GLACIER_MESH_UTILS_H_
