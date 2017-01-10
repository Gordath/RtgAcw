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
	protected:
		unsigned int m_vertex_count = 0;

		PrimitiveTopology m_primitive_topology{ PrimitiveTopology::TRIANGLE_LIST };

	public:
		virtual ~VBO() = default;

		virtual bool create(const std::vector<Vertex> &vertices, PrimitiveTopology primitive_topology) noexcept = 0;

		virtual void bind() const noexcept = 0;
		
		virtual void draw() const noexcept = 0;

		void set_primitive_topology(PrimitiveTopology primitive_topology) noexcept
		{
			m_primitive_topology = primitive_topology;
		}
	};
}

#endif
