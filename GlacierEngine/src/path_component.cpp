#include "path_component.h"
#include "object.h"
#include <algorithm>

namespace Glacier
{
	const Keyframe& PathComponent::get_keyframe(int idx) const noexcept
	{
		return m_keyframes[(idx + m_keyframes.size()) % m_keyframes.size()];
	}

	void PathComponent::setup() noexcept
	{
		std::sort(m_keyframes.begin(), m_keyframes.end(),
		          [](auto a, auto b) {
			          return std::get<long>(a) < std::get<long>(b);
		          });
	}

	void PathComponent::update(float dt, long time) noexcept
	{
		if (m_keyframes.empty()) {
			return;
		}

		Keyframe first_keyframe{ m_keyframes[0] };
		Keyframe last_keyframe{ m_keyframes[m_keyframes.size() - 1] };

		long start_time{ std::get<long>(first_keyframe) };
		long full_duration{ std::get<long>(last_keyframe) - start_time };

		// Loop it if enabled.
		if (full_duration && m_loop) {
			time -= start_time;
			time %= full_duration;
			time += start_time;
		}

		if (time < std::get<long>(first_keyframe)) {
			m_parent->set_position(std::get<Vec3f>(first_keyframe));
		}

		if (time > std::get<long>(last_keyframe)) {
			m_parent->set_position(std::get<Vec3f>(last_keyframe));
		}

		long interval{ 0 };
		float t{ 0.0 };

		for (int i = 0; i < m_keyframes.size() - 1; ++i) {
			Keyframe current{ get_keyframe(i) };
			Keyframe next{ get_keyframe(i + 1) };
			Keyframe prev{ get_keyframe(i - 1) };
			Keyframe n_next{ get_keyframe(i + 2) };

			long current_timestamp{ std::get<long>(current) };
			long next_timestamp{ std::get<long>(next) };

			if (time >= current_timestamp && time <= next_timestamp) {
				interval = next_timestamp - current_timestamp;

				t = static_cast<float>(time - current_timestamp) / static_cast<float>(interval);

				Vec3f current_pos{ std::get<Vec3f>(current) };
				Vec3f next_pos{ std::get<Vec3f>(next) };
				Vec3f prev_pos{ std::get<Vec3f>(prev) };
				Vec3f n_next_pos{ std::get<Vec3f>(n_next) };

				Vec3f pos{ MathUtils::catmull_rom(prev_pos, current_pos, next_pos, n_next_pos, t) };
				Vec3f pos2{ MathUtils::catmull_rom(prev_pos, current_pos, next_pos, n_next_pos, t + 0.05f) };

				m_parent->set_position(pos);

				if (m_align_to_path) {
					Vec3f dir_to_align{ MathUtils::normalize(pos2 - pos) };

					float rot_y = atan2f(dir_to_align.x, dir_to_align.z);
					float rot_x = asinf(dir_to_align.y);

					float rot_y_deg{ MathUtils::to_degrees(rot_y) };
					float rot_x_deg{ MathUtils::to_degrees(rot_x) };

					Vec3f angles{ m_parent->get_euler_angles() };
					angles.x = -rot_x_deg;
					angles.y = rot_y_deg;

					m_parent->set_euler_angles(angles);
				}
			}
		}
	}

	void PathComponent::teardown() noexcept
	{
	}

	void PathComponent::add_keyframe(const Vec3f& position, long time) noexcept
	{
		auto it{ m_keyframes.begin() };
		while (it != m_keyframes.end()) {
			if (std::get<long>(*it) == time) {
				std::get<Vec3f>(*it) = position;
				return;
			}

			++it;
		}

		m_keyframes.push_back(std::make_tuple(position, time));
	}
}
