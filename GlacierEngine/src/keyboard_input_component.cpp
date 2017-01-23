#include "keyboard_input_component.h"
#include "object.h"

namespace Glacier
{
	void KeyboardInputComponent::setup() noexcept
	{
		get_parent()->register_observer("msg_keypress", this);
	}


	void KeyboardInputComponent::update(float dt, long time) noexcept
	{
	}

	void KeyboardInputComponent::teardown() noexcept
	{
	}
}
