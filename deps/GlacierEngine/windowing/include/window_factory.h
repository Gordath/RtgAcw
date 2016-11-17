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
			static_assert(sizeof(T) == sizeof(Window), "Type is not a window.");
			return new T;
		}
	};
}

#endif //GLACIER_WINDOW_FACTORY_H_