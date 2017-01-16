#ifndef GLACIER_MESH_UTILS_H_
#define GLACIER_MESH_UTILS_H_
#include "../rendering/include/mesh.h"

namespace Glacier
{
	namespace MeshUtils
	{
		Mesh* generate_cube(float size, VertexWinding vertex_winding = VertexWinding::CLOCKWISE) noexcept;

		Mesh* generate_uv_sphere(float radius, int slices, int stacks, float urange = 1.0f, float vrange = 1.0f) noexcept;

		Mesh* generate_plane_xy(float size) noexcept;
	}
}

#endif //GLACIER_MESH_UTILS_H_
