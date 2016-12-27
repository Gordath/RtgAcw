#ifndef GLACIER_SYSTEM_H_
#define GLACIER_SYSTEM_H_
#include <vector>
#include <memory>
#include "object.h"

namespace Glacier
{
	class System {
	public:
		virtual ~System() = default;

		virtual void process(const std::vector<Object*>& objects, float delta_time) const noexcept = 0;
	};
}

#endif //GLACIER_SYSTEM_H_
