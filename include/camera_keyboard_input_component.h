#ifndef CAMERA_KEYBOARD_INPUT_COMPONENT_H_
#define CAMERA_KEYBOARD_INPUT_COMPONENT_H_
#include "keyboard_input_component.h"
#include "internal/math_utils.h"

class CameraKeyboardInputComponent : public Glacier::KeyboardInputComponent {
private:
	bool m_move_forwards{ false };
	bool m_move_backwards{ false };
	bool m_move_left{ false };
	bool m_move_right{ false };
	bool m_move_up{ false };
	bool m_move_down{ false };
	bool m_rotate_left{ false };
	bool m_rotate_right{ false };
	bool m_rotate_donwards{ false };
	bool m_rotate_upwards{ false };

	float m_movement_speed{ 0.0f };
	float m_rotation_speed{ 0.0f };

	float yaw{ 0.0f };

public:
	CameraKeyboardInputComponent(Glacier::Object* parent)
		: KeyboardInputComponent{ parent }
	{
	}

	void update(float dt, long time) noexcept override;

	void on_message(Glacier::MessageContainer msg) override;

	void set_movement_speed(float movement_speed) noexcept
	{
		m_movement_speed = movement_speed;
	}

	void set_rotation_speed(float rotation_speed) noexcept
	{
		m_rotation_speed = rotation_speed;
	}
};

#endif //CAMERA_KEYBOARD_INPUT_COMPONENT_H_
