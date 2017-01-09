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
					Vec4f{ -half_size, -half_size, -half_size, 1.0f },
					Vec4f{0.0f, 0.0f, -1.0f, 0.0f},
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, half_size, -half_size, 1.0f },
					Vec4f{ 0.0f, 0.0f, -1.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ half_size, -half_size, -half_size, 1.0f },
					Vec4f{ 0.0f, 0.0f, -1.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ half_size, half_size, -half_size, 1.0f },
					Vec4f{ 0.0f, 0.0f, -1.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			//right
			m->add_vertex(
				Vertex{
					Vec4f{ half_size, -half_size, -half_size, 1.0f },
					Vec4f{1.0f, 0.0f, 0.0f, 0.0f},
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ half_size, half_size, -half_size, 1.0f },
					Vec4f{ 1.0f, 0.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ half_size, -half_size, half_size, 1.0f },
					Vec4f{ 1.0f, 0.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ half_size, half_size, half_size, 1.0f },
					Vec4f{ 1.0f, 0.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			//left
			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, -half_size, half_size, 1.0f },
					Vec4f{ -1.0f, 0.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, half_size, half_size, 1.0f },
					Vec4f{ -1.0f, 0.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, -half_size, -half_size, 1.0f },
					Vec4f{ -1.0f, 0.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, half_size, -half_size, 1.0f },
					Vec4f{ -1.0f, 0.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			//back
			m->add_vertex(
				Vertex{
					Vec4f{ half_size, -half_size, half_size, 1.0f },
					Vec4f{ 0.0f, 0.0f, 1.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});
			m->add_vertex(
				Vertex{
					Vec4f{ half_size, half_size, half_size, 1.0f },
					Vec4f{ 0.0f, 0.0f, 1.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});
			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, -half_size, half_size, 1.0f },
					Vec4f{ 0.0f, 0.0f, 1.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});
			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, half_size, half_size, 1.0f },
					Vec4f{ 0.0f, 0.0f, 1.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			//top
			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, half_size, -half_size, 1.0f },
					Vec4f{ 0.0f, 1.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, half_size, half_size, 1.0f },
					Vec4f{ 0.0f, 1.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ half_size, half_size, -half_size, 1.0f },
					Vec4f{ 0.0f, 1.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ half_size, half_size, half_size, 1.0f },
					Vec4f{ 0.0f, 1.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			//bottom
			m->add_vertex(
				Vertex{
					Vec4f{ half_size, -half_size, -half_size, 1.0f },
					Vec4f{ 0.0f, -1.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ half_size, -half_size, half_size, 1.0f },
					Vec4f{ 0.0f, -1.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, -half_size, -half_size, 1.0f },
					Vec4f{ 0.0f, -1.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, -half_size, half_size, 1.0f },
					Vec4f{ 0.0f, -1.0f, 0.0f, 0.0f },
					Vec4f{},
					Vec4f{},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->generate_indices(vertex_winding);
			m->initiaze_buffer_objects();

			return m;
		}

		Mesh* generate_uv_sphere(float radius, int resolution) noexcept
		{
			if (resolution < 2) {
				resolution = 2;
			}

			Mesh* m{ new Mesh };

			float alpha, beta;       
			for (alpha = 0.0; alpha < MathUtils::PI_F; alpha += MathUtils::PI_F / resolution) {
				for (beta = 0.0; beta < 2.01 * MathUtils::PI_F; beta += MathUtils::PI_F / resolution) {
					Vertex	v1, v2;
					v1.position.x = radius * sin(beta) * sin(alpha);
					v1.position.y = radius * cos(alpha);
					v1.position.z = radius * cos(beta) * sin(alpha);

					v1.normal = v1.position;

					m->add_vertex(v1);

					v2.position.x = radius * sin(beta) * sin(alpha + MathUtils::PI_F / resolution);
					v2.position.y = radius * cos(alpha + MathUtils::PI_F / resolution);
					v2.position.z = radius * cos(beta) * sin(alpha + MathUtils::PI_F / resolution);

					v2.normal = v2.position;

					m->add_vertex(v2);
				}
			}

			m->initiaze_buffer_objects(PrimitiveTopology::TRIANGLE_STRIP);

			return m;
		}
	}
}
