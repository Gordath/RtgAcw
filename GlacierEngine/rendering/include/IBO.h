#ifndef GLACIER_ENGINE_IBO_H_
#define GLACIER_ENGINE_IBO_H_

#include "GAPI_context_locator.h"
#include <vector>

namespace Glacier
{
	class IBO : protected GAPIContextLocator {
	protected:
		unsigned int _index_count = 0;

	public:
		virtual ~IBO() = default;

		virtual bool create(const std::vector<unsigned int> &indices) = 0;

		virtual void draw() const = 0;
	};
}

#endif //GLACIER_ENGINE_IBO_H_