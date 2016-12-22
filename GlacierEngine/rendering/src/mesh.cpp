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

	void Mesh::initiaze_buffer_objects() const
	{
		if (!m_vbo) {
			m_vbo->create(m_vertices);
			m_ibo->create(m_indices);
		}
	}

	void Mesh::set_vertex_data(const Vertex* vertices, int vertex_count)
	{
		m_vertices.resize(vertex_count);
		memcpy(m_vertices.data(), vertices, sizeof(Vertex) * vertex_count);
	}

	Vertex* Mesh::get_vertex_data() const
	{
		return const_cast<Vertex*>(m_vertices.data());
	}

	void Mesh::add_vertex(const Vertex& vertex)
	{
		m_vertices.push_back(vertex);
	}

	void Mesh::set_index_data(const unsigned int* indices, int index_count)
	{
		m_indices.resize(index_count);
		memcpy(m_indices.data(), indices, sizeof(unsigned int) * index_count);
	}

	unsigned int* Mesh::get_index_data() const
	{
		return const_cast<unsigned int*>(m_indices.data());
	}

	void Mesh::add_index(unsigned int index)
	{
		m_indices.push_back(index);
	}

	void Mesh::draw() const
	{
		m_vbo->draw();
	}

	void Mesh::draw_indexed() const
	{
		m_ibo->draw();
	}
}
