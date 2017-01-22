#ifndef CAMERA_KEYBOARD_INPUT_COMPONENT_H_
#define CAMERA_KEYBOARD_INPUT_COMPONENT_H_
#include "keyboard_input_component.h"
#include "internal/math_utils.h"

class CameraKeyboardInputComponent : public Glacier::KeyboardInputComponent
{
private:
	bool m_move_forwards;
	bool m_move_backwards;
	bool m_move_left;
	bool m_move_right;
	bool m_move_up;
	bool m_move_down;
	bool m_rotate_left;
	bool m_rotate_right;
	bool m_rotate_donwards;
	bool m_rotate_upwards;

	float m_movement_speed;
	float m_rotation_speed;

public:
	explicit CameraKeyboardInputComponent(Glacier::Object* parent)
		: KeyboardInputComponent{parent},
		  m_move_forwards{false},
		  m_move_backwards{false},
		  m_move_left{false},
		  m_move_right{false},
		  m_move_up{false},
		  m_move_down{false},
		  m_rotate_left{false},
		  m_rotate_right{false},
		  m_rotate_donwards{false},
		  m_rotate_upwards{false},
		  m_movement_speed{0.0f},
		  m_rotation_speed{0.0f}
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
