#include "component.h"
#include "object.h"

namespace Glacier
{
	Component::Component(const std::string& type, Object* parent) : m_type{ type }, m_parent{ parent }
	{
		parent->add_component(this);
	}
}
