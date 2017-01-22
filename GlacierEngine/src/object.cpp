#include "object.h"

namespace Glacier
{
	Object::~Object()
	{
		try {
			for (Component* component : m_components) {
				delete component;
			}
		}
		catch (...)
		{
		}
		m_components.clear();
	}
}
