#ifndef DREBEL_SUB_H_
#define DREBEL_SUB_H_
#include "scene.h"
#include "object.h"


class DrebelSubmarine : public Glacier::Object {
private:
	Glacier::Scene* m_scene{ nullptr };

	Object* m_oar_controller; /* parasoft-suppress  MRM-33 "This pointer is just a handle. It is added to the scene in the setup method of the class and the Scene manages the memory." */

public:
	DrebelSubmarine(const std::string& name, Glacier::Scene* scene)
		: Object{ name }, m_scene{ scene }, m_oar_controller{ nullptr }
	{
	}

	DrebelSubmarine(const DrebelSubmarine& other)
		: Glacier::Object{other},
		  m_scene{other.m_scene},
		  m_oar_controller{other.m_oar_controller}
	{
	}

	explicit DrebelSubmarine(DrebelSubmarine&& other) noexcept
		: Glacier::Object{std::move(other)},
		  m_scene{other.m_scene},
		  m_oar_controller{other.m_oar_controller}
	{
	}

	DrebelSubmarine& operator=(const DrebelSubmarine& other)
	{
		if (this == &other)
			return *this;
		Glacier::Object::operator =(other);
		m_scene = other.m_scene;
		m_oar_controller = other.m_oar_controller;
		return *this;
	}

	DrebelSubmarine& operator=(DrebelSubmarine&& other) noexcept
	{
		if (this == &other)
			return *this;
		Glacier::Object::operator =(std::move(other));
		m_scene = other.m_scene;
		m_oar_controller = other.m_oar_controller;
		return *this;
	}

	void update(float dt, long time) noexcept override;

	void setup() noexcept override;
};

#endif //DREBEL_SUB_H_
