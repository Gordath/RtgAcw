#include "mesh.h"
#include "D3D11_VBO.h"
#include "D3D11_IBO.h"

namespace Glacier
{
	Mesh::Mesh()
	{
#ifdef GLACIERENGINE_BUILD_D3D
		_vbo = std::make_unique<D3D11VBO>();
		_ibo = std::make_unique<D3D11IBO>();
#else
		//GL versions here
#endif
	}

	void Mesh::initiaze_buffer_objects() const
	{
		if (!_vbo) {
			_vbo->create(_vertices);
			_ibo->create(_indices);
		}
	}

	void Mesh::set_vertex_data(const Vertex *vertices, int vertex_count)
	{
		_vertices.resize(vertex_count);
		memcpy(_vertices.data(), vertices, sizeof(Vertex) * vertex_count);
	}

	Vertex* Mesh::get_vertex_data() const
	{
		return const_cast<Vertex*>(_vertices.data());
	}

	void Mesh::add_vertex(const Vertex &vertex)
	{
		_vertices.push_back(vertex);
	}

	void Mesh::set_index_data(const unsigned int *indices, int index_count)
	{
		_indices.resize(index_count);
		memcpy(_indices.data(), indices, sizeof(unsigned int) * index_count);
	}

	unsigned int* Mesh::get_index_data() const
	{
		return const_cast<unsigned int*>(_indices.data());
	}

	void Mesh::add_index(unsigned int index)
	{
		_indices.push_back(index);
	}

	void Mesh::draw() const
	{
		_vbo->draw();
	}

	void Mesh::draw_indexed() const
	{
		_ibo->draw();
	}

}
