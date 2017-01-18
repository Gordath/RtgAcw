#ifndef DREBEL_SUB_H_
#define DREBEL_SUB_H_
#include "scene.h"
#include "object.h"


class DrebelSubmarine : public Glacier::Object {
private:
	Glacier::Scene* m_scene{ nullptr };

	Glacier::Object* m_oar_controller{ nullptr };

public:
	DrebelSubmarine(const std::string& name, Glacier::Scene* scene)
		: Object{ name }, m_scene{ scene }
	{
	}

	void update(float dt, long time) noexcept override;

	void setup() noexcept override;
};

#endif //DREBEL_SUB_H_
