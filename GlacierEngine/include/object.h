#ifndef GLACIER_OBJECT_H_
#define GLACIER_OBJECT_H_
#include <vector>
#include "component.h"
#include <memory>

namespace Glacier
{
	class Object {
	private:
		std::vector<Component*> m_components;

		std::string m_name;

		bool alive{ true };

	public:
		Object(const std::string& name) : m_name{ name }
		{
		}

		Component* get_component(const std::string& type) const noexcept
		{
			for (const auto component : m_components) {
				if (component->get_type() == type) {
					return component;
				}
			}

			return nullptr;
		}

		bool is_alive() const noexcept
		{
			return alive;
		}
	};
}

#endif //GLACIER_OBJECT_H_
