#ifndef GLACIER_KEYBOARD_INPUT_COMPONENT
#define GLACIER_KEYBOARD_INPUT_COMPONENT
#include "component.h"

namespace Glacier
{
	class KeyboardInputComponent : public Component {
	public:
		explicit KeyboardInputComponent(Object* parent)
			: Component{ "co_keyboard_input", parent }
		{
		}

		void setup() noexcept override;

		void update(float dt, long time) noexcept override {};

		void teardown() noexcept override {};
	};
}

#endif //GLACIER_KEYBOARD_INPUT_COMPONENT
