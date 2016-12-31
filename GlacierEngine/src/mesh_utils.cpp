#include "mesh_utils.h"

namespace Glacier
{
	namespace MeshUtils
	{
		Mesh* generate_cube()
		{
			Mesh* m{ new Mesh };

			m->add_vertex(
				Vertex{
					Vec3f{ 0.0f, 0.5f, 0.0f },
					Vec3f{},
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
				});

			m->add_vertex(
				Vertex{
					Vec3f{ 0.45f, -0.5, 0.0f },
					Vec3f{},
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
				});

			m->add_vertex(
				Vertex{
					Vec3f{ -0.45f, -0.5f, 0.0f },
					Vec3f{},
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
				});

			//			m->add_vertex(
			//				Vertex{
			//					Vec3f{ -0.5f, -0.5f, -0.5f },
			//					Vec3f{},
			//					Vec3f{},
			//					Vec2f{},
			//					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			//				});
			//
			//			m->add_vertex(
			//				Vertex{
			//					Vec3f{ -0.5f, 0.5f, -0.5f },
			//					Vec3f{},
			//					Vec3f{},
			//					Vec2f{},
			//					Vec4f{ 1.0f, 0.0f,0.0f, 1.0f }
			//				});
			//
			//			m->add_vertex(
			//				Vertex{
			//					Vec3f{ 0.5f, -0.5f, -0.5f },
			//					Vec3f{},
			//					Vec3f{},
			//					Vec2f{},
			//					Vec4f{ 1.0f, 0.0f,0.0f, 1.0f }
			//				});
			//
			//			m->add_vertex(
			//				Vertex{
			//					Vec3f{ 0.5f, 0.5f, -0.5f },
			//					Vec3f{},
			//					Vec3f{},
			//					Vec2f{},
			//					Vec4f{ 1.0f, 0.0f,0.0f, 1.0f }
			//				});

			m->generate_indices(VertexWinding::CLOCKWISE);
			m->initiaze_buffer_objects();

			return m;
		}
	}
}
