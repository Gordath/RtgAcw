#include "path_component.h"
#include "object.h"
#include <algorithm>

namespace Glacier
{
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
			Keyframe current{ m_keyframes[i] };
			Keyframe next{ m_keyframes[i + 1] };

			long current_timestamp{ std::get<long>(current) };
			long next_timestamp{ std::get<long>(next) };

			if (time >= current_timestamp && time <= next_timestamp) {
				interval = next_timestamp - current_timestamp;

				t = static_cast<float>(time - current_timestamp) / static_cast<float>(interval);

				Vec3f current_pos{ std::get<Vec3f>(current) };
				Vec3f next_pos{ std::get<Vec3f>(next) };

				m_parent->set_position(
					Vec3f{
						current_pos.x + (next_pos.x - current_pos.x) * t,
						current_pos.y + (next_pos.y - current_pos.y) * t,
						current_pos.z + (next_pos.z - current_pos.z) * t
					}
				);
			}
		}
	}

	void PathComponent::teardown() noexcept
	{
	}

	void PathComponent::add_keyframe(const Vec3f& position, long time) noexcept
	{
		auto it{ m_keyframes.begin() };
		while(it != m_keyframes.end()) {
			if (std::get<long>(*it) == time) {
				std::get<Vec3f>(*it) = position;
				return;
			}

			++it;
		}

		m_keyframes.push_back(std::make_tuple(position, time));
	}
}
