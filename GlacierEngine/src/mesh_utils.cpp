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
					Vec4f{0.0f, 1.0f, 0.0f, 1.0f},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, half_size, -half_size, 1.0f },
					Vec4f{ 0.0f, 0.0f, -1.0f, 0.0f },
					Vec4f{},
					Vec4f{0.0f, 0.0f, 0.0f, 1.0f},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ half_size, -half_size, -half_size, 1.0f },
					Vec4f{ 0.0f, 0.0f, -1.0f, 0.0f },
					Vec4f{},
					Vec4f{1.0f, 1.0f, 0.0f, 1.0f},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ half_size, half_size, -half_size, 1.0f },
					Vec4f{ 0.0f, 0.0f, -1.0f, 0.0f },
					Vec4f{},
					Vec4f{1.0f, 0.0f, 0.0f, 1.0f},
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
					Vec4f{1.0f, 0.0f, 0.0f, 1.0f},
					Vec4f{0.0f, 1.0f, 0.0f, 1.0f},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ -half_size, half_size, half_size, 1.0f },
					Vec4f{ 0.0f, 1.0f, 0.0f, 0.0f },
					Vec4f{1.0f, 0.0f, 0.0f, 1.0f},
					Vec4f{0.0f, 0.0f, 0.0f, 1.0f},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ half_size, half_size, -half_size, 1.0f },
					Vec4f{ 0.0f, 1.0f, 0.0f, 0.0f },
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f },
					Vec4f{1.0f, 1.0f, 0.0f, 1.0f},
					Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
					Vec4f{ half_size, half_size, half_size, 1.0f },
					Vec4f{ 0.0f, 1.0f, 0.0f, 0.0f },
					Vec4f{1.0f, 0.0f, 0.0f, 1.0f},
					Vec4f{1.0f, 0.0f, 0.0f, 1.0f},
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

		Mesh* generate_uv_sphere(float radius, int slices, int stacks, float urange, float vrange) noexcept
		{
			Mesh* m{ new Mesh };

			Vertex tmp_vert;

			if (slices < 4) {
				slices = 4;
			}
				
			if (stacks < 2) {
				stacks = 2;
			}

			int u_verts = slices + 1;
			int v_verts = stacks + 1;

			float du = urange / static_cast<float>(u_verts - 1);
			float dv = vrange / static_cast<float>(v_verts - 1);

			float u = 0.0;

			for (int i = 0; i < u_verts; i++) {
				float theta = (u * 2.0 * MathUtils::PI_F) * urange;

				float v = 0.0;
				for (int j = 0; j < v_verts; j++) {
					float phi = v * MathUtils::PI_F * vrange;
					tmp_vert.position = Vec4f{ MathUtils::spherical_to_cartesian(theta, phi, radius), 1.0f };
					tmp_vert.normal = tmp_vert.position;
					tmp_vert.tangent = MathUtils::normalize(
						Vec4f{ 
							(MathUtils::spherical_to_cartesian(theta + 1.0, MathUtils::PI_F / 2.0f)
							    - MathUtils::spherical_to_cartesian(theta - 1.0, MathUtils::PI_F / 2.0f))
								, 1.0f 
						});

					tmp_vert.texcoord.x = u * urange;
					tmp_vert.texcoord.y = v * vrange;

					if (i < slices && j < stacks) {
						int idx = i * v_verts + j;

						int idx1 = idx;
						int idx2 = idx + 1;
						int idx3 = idx + v_verts + 1;

						int idx4 = idx;
						int idx5 = idx + v_verts + 1;
						int idx6 = idx + v_verts;

						m->add_index(idx1);
						m->add_index(idx2);
						m->add_index(idx3);
						m->add_index(idx4);
						m->add_index(idx5);
						m->add_index(idx6);
					}

					m->add_vertex(tmp_vert);

					v += dv;
				}
				u += du;
			}

			m->initiaze_buffer_objects();
			return m;
		}

		Mesh* generate_plane_xy(float size) noexcept
		{
			Mesh* m{ new Mesh };

			float half_size{ size / 2.0f };

			//front
			m->add_vertex(
				Vertex{
				Vec4f{ -half_size, -half_size, 0.0f, 1.0f },
				Vec4f{ 0.0f, 0.0f, -1.0f, 0.0f },
				Vec4f{},
				Vec4f{0.0f, 1.0f, 0.0f, 1.0f},
				Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
				Vec4f{ -half_size, half_size, 0.0f, 1.0f },
				Vec4f{ 0.0f, 0.0f, -1.0f, 0.0f },
				Vec4f{},
				Vec4f{ 0.0f, 0.0f, 0.0f, 1.0f},
				Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
				Vec4f{ half_size, -half_size, 0.0f, 1.0f },
				Vec4f{ 0.0f, 0.0f, -1.0f, 0.0f },
				Vec4f{},
				Vec4f{1.0f, 1.0f, 0.0f, 1.0f},
				Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->add_vertex(
				Vertex{
				Vec4f{ half_size, half_size, 0.0f, 1.0f },
				Vec4f{ 0.0f, 0.0f, -1.0f, 0.0f },
				Vec4f{},
				Vec4f{1.0f, 0.0f, 0.0f, 1.0f},
				Vec4f{ 1.0f, 0.0f, 0.0f, 1.0f }
			});

			m->generate_indices(VertexWinding::CLOCKWISE);
			m->initiaze_buffer_objects();
			return m;
		}
	}
}
