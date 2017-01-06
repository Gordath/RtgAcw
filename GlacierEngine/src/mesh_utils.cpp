#include "mesh_utils.h"
#include "internal/math_utils.h"

namespace Glacier
{
	namespace MeshUtils
	{
		Mesh* generate_cube(float size, VertexWinding vertex_winding) noexcept
		{
			Mesh* m{ new Mesh };

			float half_size{ size / 2.0f };

			//front
			m->add_vertex(
				Vertex{
					Vec3f{ -half_size, -half_size, -half_size },
					Vec3f{0.0f, 0.0f, -1.0f},
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ -half_size, half_size, -half_size },
					Vec3f{ 0.0f, 0.0f, -1.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ half_size, -half_size, -half_size },
					Vec3f{ 0.0f, 0.0f, -1.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ half_size, half_size, -half_size },
					Vec3f{ 0.0f, 0.0f, -1.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			//right
			m->add_vertex(
				Vertex{
					Vec3f{ half_size, -half_size, -half_size },
					Vec3f{1.0f, 0.0f, 0.0f},
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ half_size, half_size, -half_size },
					Vec3f{ 1.0f, 0.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ half_size, -half_size, half_size },
					Vec3f{ 1.0f, 0.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ half_size, half_size, half_size },
					Vec3f{ 1.0f, 0.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			//left
			m->add_vertex(
				Vertex{
					Vec3f{ -half_size, -half_size, half_size },
					Vec3f{ -1.0f, 0.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ -half_size, half_size, half_size },
					Vec3f{ -1.0f, 0.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ -half_size, -half_size, -half_size },
					Vec3f{ -1.0f, 0.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ -half_size, half_size, -half_size },
					Vec3f{ -1.0f, 0.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			//back
			m->add_vertex(
				Vertex{
					Vec3f{ half_size, -half_size, half_size },
					Vec3f{ 0.0f, 0.0f, 1.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});
			m->add_vertex(
				Vertex{
					Vec3f{ half_size, half_size, half_size },
					Vec3f{ 0.0f, 0.0f, 1.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});
			m->add_vertex(
				Vertex{
					Vec3f{ -half_size, -half_size, half_size },
					Vec3f{ 0.0f, 0.0f, 1.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});
			m->add_vertex(
				Vertex{
					Vec3f{ -half_size, half_size, half_size },
					Vec3f{ 0.0f, 0.0f, 1.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			//top
			m->add_vertex(
				Vertex{
					Vec3f{ -half_size, half_size, -half_size },
					Vec3f{ 0.0f, 1.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ -half_size, half_size, half_size },
					Vec3f{ 0.0f, 1.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ half_size, half_size, -half_size },
					Vec3f{ 0.0f, 1.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ half_size, half_size, half_size },
					Vec3f{ 0.0f, 1.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			//bottom
			m->add_vertex(
				Vertex{
					Vec3f{ half_size, -half_size, -half_size },
					Vec3f{ 0.0f, -1.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ half_size, -half_size, half_size },
					Vec3f{ 0.0f, -1.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ -half_size, -half_size, -half_size },
					Vec3f{ 0.0f, -1.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec3f{ -half_size, -half_size, half_size },
					Vec3f{ 0.0f, -1.0f, 0.0f },
					Vec3f{},
					Vec2f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->generate_indices(vertex_winding);
			m->initiaze_buffer_objects();

			return m;
		}

		Mesh* generate_uv_sphere(float radius,
			int slices,
			int stacks,
			float u_range,
			float v_range,
			VertexWinding vertex_winding) noexcept
		{
			//TODO: generate a freaking sphere.

			if (slices < 4) {
				slices = 4;
			}

			if (stacks < 2) {
				stacks = 2;
			}

			int uverts{ slices + 1 };
			int vverts{ stacks + 1 };

			int num_verts{ uverts * vverts };
			int num_quads{ slices * stacks };
			int num_tri{ num_quads * 2 };


			float du = u_range / static_cast<float>(uverts - 1);
			float dv = v_range / static_cast<float>(vverts - 1);

			float u = 0.0f;
			for (int i = 0; i < uverts; i++) {
				float theta = u * 2.0 * MathUtils::PI_F;

				float v = 0.0;
				for (int j = 0; j < vverts; j++) {
					float phi = v * MathUtils::PI_F;

					Vec3f position{ MathUtils::spherical_to_cartesian(theta, phi) };

					Vec3f normal{ position };

					Vec3f tangent{
						MathUtils::normalize(
							MathUtils::spherical_to_cartesian(theta + 0.1f, MathUtils::PI_F / 2.0f) -
							MathUtils::spherical_to_cartesian(theta - 0.1f, MathUtils::PI_F / 2.0f)
						)
					};

					Vec2f texcoord{ u * u_range, v * v_range };
				}
			}
			return nullptr;
		}
	}
}
