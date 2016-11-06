#ifndef GLACIER_WINDOW_FACTORY_H_
#define GLACIER_WINDOW_FACTORY_H_

#include "window.h"

namespace Glacier
{
	template<typename T>
	class WindowFactory {
	public:
		static T *create_instance()
		{
			return new T;
		}
	};
}

#endif //GLACIER_WINDOW_FACTORY_H_