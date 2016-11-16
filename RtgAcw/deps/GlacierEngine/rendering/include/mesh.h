#ifndef GLACIER_ENGINE_MESH_H_
#define GLACIER_ENGINE_MESH_H_

#include <vector>
#include "vertex.h"
#include "VBO.h"
#include "IBO.h"

namespace Glacier
{
	class Mesh {
	private:
		std::vector<Vertex>			_vertices;
		std::vector<unsigned int>	_indices;

		std::unique_ptr<VBO>		_vbo;
		std::unique_ptr<IBO>		_ibo;

		const std::string			_name;

	public:
		Mesh();

		void initiaze_buffer_objects() const;

		void set_vertex_data(const Vertex *vertices, int vertex_count);

		Vertex* get_vertex_data() const;

		void add_vertex(const Vertex &vertex);

		void set_index_data(const unsigned int *indices, int index_count);

		unsigned int* get_index_data() const;

		void add_index(unsigned int index);

		void draw() const;

		void draw_indexed() const;
	};
}

#endif //GLACIER_ENGINE_MESH_H_