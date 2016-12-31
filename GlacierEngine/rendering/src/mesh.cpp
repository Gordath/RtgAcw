#include "mesh.h"
#include "D3D11_VBO.h"
#include "D3D11_IBO.h"

namespace Glacier
{
	Mesh::Mesh()
	{
#ifdef GLACIERENGINE_BUILD_D3D
		m_vbo = std::make_unique<D3D11VBO>();
		m_ibo = std::make_unique<D3D11IBO>();
#else
		//GL versions here
#endif
	}

	void Mesh::initiaze_buffer_objects() const noexcept
	{
		if (m_vertices.size()) {
			m_vbo->create(m_vertices);
		}

		if (m_indices.size()) {
			m_ibo->create(m_indices);
		}
	}

	void Mesh::set_vertex_data(const Vertex* vertices, int vertex_count) noexcept
	{
		m_vertices.resize(vertex_count);
		memcpy(m_vertices.data(), vertices, sizeof(Vertex) * vertex_count);
	}

	Vertex* Mesh::get_vertex_data() const noexcept
	{
		return const_cast<Vertex*>(m_vertices.data());
	}

	void Mesh::add_vertex(const Vertex& vertex) noexcept
	{
		m_vertices.push_back(vertex);
	}

	void Mesh::set_index_data(const unsigned int* indices, int index_count) noexcept
	{
		m_indices.resize(index_count);
		memcpy(m_indices.data(), indices, sizeof(unsigned int) * index_count);
	}

	unsigned int* Mesh::get_index_data() const noexcept
	{
		return const_cast<unsigned int*>(m_indices.data());
	}

	void Mesh::add_index(unsigned int index) noexcept
	{
		m_indices.push_back(index);
	}

	bool Mesh::load(const std::wstring& file_name) noexcept
	{
		//TODO: implement loading.
		return true;
	}

	void Mesh::generate_indices(VertexWinding winding) noexcept
	{
		if (m_indices.size()) {
			return;
		}

		int quad_count = m_vertices.size() / 4;
		int triangle_count = quad_count * 2;

		m_indices.resize(triangle_count * 3);

		for (int i = 0, j = 0; i < m_indices.size(); i += 6 , j += 4) {
			m_indices[i] = j;
			switch (winding) {
			case VertexWinding::CLOCKWISE:
				m_indices[i + 1] = m_indices[i + 4] = j + 1;
				m_indices[i + 2] = m_indices[i + 3] = j + 2;
				break;
			case VertexWinding::ANTICLOCKWISE:
				m_indices[i + 1] = m_indices[i + 4] = j + 2;
				m_indices[i + 2] = m_indices[i + 3] = j + 1;
				break;
			default:
				break;
			}

			m_indices[i + 5] = j + 3;
		}
	}
}
