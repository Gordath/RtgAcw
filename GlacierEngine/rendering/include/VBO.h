#ifndef GLACIER_ENGINE_VBO_H_
#define GLACIER_ENGINE_VBO_H_
#include <vector>
#include "vertex.h"

namespace Glacier
{
	class VBO {
	protected:
		unsigned int m_vertex_count = 0;

	public:
		virtual ~VBO() = default;

		virtual bool create(const std::vector<Vertex> &vertices) = 0;
		
		virtual void draw() const = 0;
	};
}

#endif
