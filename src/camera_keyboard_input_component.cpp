#include "camera_keyboard_input_component.h"
#include "keypress_message.h"
#include <iostream>
#include "object.h"

using namespace Glacier;

void CameraKeyboardInputComponent::update(float dt, long time) noexcept
{
	Vec3f angles{ m_parent->get_euler_angles() };
	std::cout << angles.y << std::endl;
	float distance{ m_movement_speed * dt };
	float rot = m_rotation_speed * dt;
	float dx = sinf(MathUtils::to_radians(-angles.y)) * distance;
	float dz = cosf(MathUtils::to_radians(-angles.y)) * distance;
	
	Vec3f pos;
	if (m_move_forwards) {
		pos.x -= dx;
		pos.z += dz;
	}

	if (m_move_backwards) {
		pos.x += dx;
		pos.z -= dz;
	}

	if (m_move_up) {
		pos.y += distance;
	}

	if (m_move_down) {
		pos.y -= distance;
	}

	if (m_rotate_left) {
		angles.y -= rot;
	}

	if (m_rotate_right) {
		angles.y += rot;
	}

	m_parent->set_position(m_parent->get_position() + pos);
	m_parent->set_euler_angles(angles);
}

void CameraKeyboardInputComponent::on_message(MessageContainer msg)
{
	Message* m{ msg.get() };
	if (m->get_message_type() == "msg_keypress") {
		KeypressMessage* kpm{ dynamic_cast<KeypressMessage*>(m) };

		if (kpm) {
			if (kpm->is_pressed()) {
				switch (kpm->get_key()) {
				case 'A':
				case 'a':
					m_rotate_left = true;
					break;
				case 'S':
				case 's':
					m_move_backwards = true;
					break;
				case 'W':
				case 'w':
					m_move_forwards = true;
					break;
				case 'D':
				case 'd':
					m_rotate_right = true;
					break;
				default:
					break;
				}
			} else {
				switch (kpm->get_key()) {
				case 'A':
				case 'a':
					m_rotate_left = false;
					break;
				case 'S':
				case 's':
					m_move_backwards = false;
					break;
				case 'W':
				case 'w':
					m_move_forwards = false;
					break;
				case 'D':
				case 'd':
					m_rotate_right = false;
					break;
				default:
					break;
				}
			}
			
		}
	}
}
