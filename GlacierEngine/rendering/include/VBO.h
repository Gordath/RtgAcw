#ifndef GLACIER_ENGINE_VBO_H_
#define GLACIER_ENGINE_VBO_H_

#include "GAPI_context_locator.h"
#include <vector>
#include "vertex.h"

namespace Glacier
{
	class VBO : protected GAPIContextLocator {
	protected:
		unsigned int _vertex_count = 0;

	public:
		virtual ~VBO() = default;

		virtual bool create(const std::vector<Vertex> &vertices) = 0;
		
		virtual void draw() const = 0;
	};
}

#endif
