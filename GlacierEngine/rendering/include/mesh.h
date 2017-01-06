#ifndef GLACIER_ENGINE_MESH_H_
#define GLACIER_ENGINE_MESH_H_

#include <vector>
#include "vertex.h"
#include "VBO.h"
#include "IBO.h"
#include <memory>
#include "resource.h"

namespace Glacier
{
	enum class VertexWinding {
		CLOCKWISE,
		ANTICLOCKWISE
	};

	class Mesh : public Resource {
	private:
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

		std::unique_ptr<VBO> m_vbo;
		std::unique_ptr<IBO> m_ibo;

		const std::string m_name;

	public:
		Mesh();

		VBO* get_vbo() const noexcept
		{
			return m_vbo.get();
		}

		IBO* get_ibo() const noexcept
		{
			return m_ibo.get();
		}

		size_t get_vertex_count() const noexcept
		{
			return m_vertices.size();
		}

		size_t get_index_count() const noexcept
		{
			return m_indices.size();
		}

		void initiaze_buffer_objects(PrimitiveTopology primitive_topology = PrimitiveTopology::TRIANGLE_LIST) const noexcept;

		void set_vertex_data(const Vertex* vertices, int vertex_count) noexcept;

		Vertex* get_vertex_data() const noexcept;

		void add_vertex(const Vertex& vertex) noexcept;

		void set_index_data(const unsigned int* indices, int index_count) noexcept;

		unsigned int* get_index_data() const noexcept;

		void add_index(unsigned int index) noexcept;

		bool load(const std::wstring& file_name) noexcept override;

		void generate_indices(VertexWinding winding) noexcept;
	};
}

#endif //GLACIER_ENGINE_MESH_H_
