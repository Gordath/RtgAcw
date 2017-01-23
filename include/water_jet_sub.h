#ifndef WATER_JET_SUBMARINE_H_
#define WATER_JET_SUBMARINE_H_
#include "scene.h"
#include "object.h"

class WaterJetSubmarine : public Glacier::Object {
private:
	Glacier::Scene* m_scene{ nullptr };

public:
	WaterJetSubmarine(const std::string& name, Glacier::Scene* scene)
		: Object{ name }, m_scene{ scene }
	{
	}

	WaterJetSubmarine(const WaterJetSubmarine& other) = default;
	WaterJetSubmarine& operator=(const WaterJetSubmarine& other) = default;

	void update(float dt, long time) noexcept override;

	void setup() noexcept override;
};

#endif //WATER_JET_SUBMARINE_H_
