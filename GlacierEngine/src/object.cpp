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

	Component* Object::get_component(const std::string& type) const noexcept
	{
		for (const auto component : m_components) {
			if (component->get_type() == type) {
				return component;
			}
		}

		return nullptr;
	}

	void Object::update(float dt, long time) noexcept
	{
		calculate_xform();

		for (auto component : m_components) {
			component->update(dt, time);
		}
	}

	void Object::setup() noexcept
	{
		for (auto component : m_components) {
			component->setup();
		}
	}

	void Object::teardown() noexcept
	{
		for (auto component : m_components) {
			component->teardown();
		}
	}
}
