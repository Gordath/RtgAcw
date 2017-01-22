#include "component.h"
#include "scene_manager.h"
#include "object.h"

namespace Glacier
{
	Component::~Component()
	{
		
	}

	Component::Component(const std::string& type, Object* parent) : m_type{ type }, m_parent{ parent }
	{
		parent->add_component(this);
	}

	void Component::on_message(const MessageContainer& msg)
	{
		SceneManager::on_message(msg);
	}
}
