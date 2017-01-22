#ifndef GLACIER_ENGINE_VBO_H_
#define GLACIER_ENGINE_VBO_H_
#include <vector>
#include "vertex.h"

namespace Glacier
{
	enum class PrimitiveTopology {
		TRIANGLE_LIST,
		TRIANGLE_STRIP
	};

	class VBO {
	private:
		unsigned int m_vertex_count = 0;

		PrimitiveTopology m_primitive_topology{ PrimitiveTopology::TRIANGLE_LIST };

	public:
		VBO() = default;

		virtual ~VBO() = default;

		void set_vertex_count(const unsigned int vertex_count) noexcept
		{
			m_vertex_count = vertex_count;
		}

		unsigned int get_vertex_count() const noexcept
		{
			return m_vertex_count;
		}

		void set_primitive_topology(PrimitiveTopology primitive_topology) noexcept
		{
			m_primitive_topology = primitive_topology;
		}

		PrimitiveTopology get_primitive_topology() const noexcept
		{
			return m_primitive_topology;
		}

		virtual bool create(const std::vector<Vertex> &vertices, PrimitiveTopology primitive_topology) noexcept = 0;

		virtual void bind() const noexcept = 0;
		
		virtual void draw() const noexcept = 0;
	};
}

#endif
