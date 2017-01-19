#include "camera_keyboard_input_component.h"
#include "keypress_message.h"
#include <iostream>
#include "object.h"

using namespace Glacier;

void CameraKeyboardInputComponent::update(float dt, long time) noexcept
{
	Vec3f angles{ m_parent->get_euler_angles() };
	Vec3f pos{ m_parent->get_position() };

	float distance{ m_movement_speed * dt };
	float rot = m_rotation_speed * dt;
	float dx = sinf(MathUtils::to_radians(-angles.y)) * distance;
	float dz = cosf(MathUtils::to_radians(-angles.y)) * distance;

	if (m_move_forwards) {
		pos.x -= dx;
		pos.z += dz;
	}

	if (m_move_backwards) {
		pos.x += dx;
		pos.z -= dz;
	}

	if (m_move_left) {
		pos.x -= dz;
		pos.z -= dx;
	}

	if (m_move_right) {
		pos.x += dz;
		pos.z += dx;
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

	if (m_rotate_donwards) {
		angles.x += rot;
	}

	if (m_rotate_upwards) {
		angles.x -= rot;
	}

	// restrictions
	if (pos.y < 0.0f) {
		pos.y = 0.0f;
	}

	if (pos.y > 30.0f) {
		pos.y = 30.0f;
	}

	if (angles.x < -90.0f) {
		angles.x = -90.0f;
	}

	if (angles.x > 90.0f) {
		angles.x = 90.0f;
	}
	//---------------------

	m_parent->set_position(pos);
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
					m_rotate_donwards = true;
					break;
				case 'W':
				case 'w':
					m_rotate_upwards = true;
					break;
				case 'D':
				case 'd':
					m_rotate_right = true;
					break;
				case 'J':
				case 'j':
					m_move_left = true;
					break;
				case 'L':
				case 'l':
					m_move_right = true;
					break;
				case 'I':
				case 'i':
					m_move_forwards = true;
					break;
				case 'K':
				case 'k':
					m_move_backwards = true;
					break;
				case 38:
					m_move_up = true;
					break;
				case 40:
					m_move_down = true;
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
					m_rotate_donwards = false;
					break;
				case 'W':
				case 'w':
					m_rotate_upwards = false;
					break;
				case 'D':
				case 'd':
					m_rotate_right = false;
					break;
				case 'J':
				case 'j':
					m_move_left = false;
					break;
				case 'L':
				case 'l':
					m_move_right = false;
					break;
				case 'I':
				case 'i':
					m_move_forwards = false;
					break;
				case 'K':
				case 'k':
					m_move_backwards = false;
					break;
				case 38:
					m_move_up = false;
					break;
				case 40:
					m_move_down = false;
					break;
				default:
					break;
				}
			}
			
		}
	}
}
