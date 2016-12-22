#ifndef GLACIER_OBJECT_H_
#define GLACIER_OBJECT_H_
#include <vector>
#include "component.h"
#include <memory>

namespace Glacier
{
	class Object {
	private:
		std::vector<std::unique_ptr<Component>> m_components;

		std::string m_name;

	public:
		Object(const std::string& name) : m_name{ name }
		{
		}

	};
}

#endif //GLACIER_OBJECT_H_
