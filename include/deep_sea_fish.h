#ifndef DEEP_SEA_FISH_H_
#define DEEP_SEA_FISH_H_
#include "object.h"
#include "scene.h"

class DeepSeaFish : public Glacier::Object {
private:
	Glacier::Scene* m_scene{nullptr};
	Object* m_left_fin_controller; /* parasoft-suppress  MRM-33 "This pointer is just a handle. It is added to the scene in the setup method of the class and the Scene manages the memory." */
	Object* m_right_fin_controller; /* parasoft-suppress  MRM-33 "This pointer is just a handle. It is added to the scene in the setup method of the class and the Scene manages the memory." */
	Object* m_tail_controller; /* parasoft-suppress  MRM-33 "This pointer is just a handle. It is added to the scene in the setup method of the class and the Scene manages the memory." */

public:
	DeepSeaFish(const std::string& name, Glacier::Scene* scene)
		: Object{name}, m_scene{scene}, m_left_fin_controller{nullptr},
		  m_right_fin_controller{nullptr},
		  m_tail_controller{nullptr}
	{
	}

	DeepSeaFish(const DeepSeaFish& other) = default;
	DeepSeaFish& operator=(const DeepSeaFish& other) = default;


	void update(float dt, long time) noexcept override;
	void setup() noexcept override;
	void teardown() noexcept override;
};

#endif //DEEP_SEA_FISH_H_
