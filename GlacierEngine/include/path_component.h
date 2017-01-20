#ifndef GLACIER_PATH_COMPONENT_H_
#define GLACIER_PATH_COMPONENT_H_
#include "component.h"
#include <vector>
#include <tuple>

namespace Glacier
{
	using Keyframe = std::tuple<Vec3f, long>;

	class PathComponent : public Component {
	private:
		std::vector<Keyframe> m_keyframes;

		bool m_loop{ false };
		
		bool m_align_to_path{ false };

		const Keyframe& get_keyframe(int idx) const noexcept;

	public:
		PathComponent(Object* parent) : Component{ "co_path", parent }
		{
		}

		void setup() noexcept override;

		void update(float dt, long time) noexcept override;

		void teardown() noexcept override;

		void add_keyframe(const Vec3f& position, long time) noexcept;

		void set_looping(bool looping) noexcept
		{
			m_loop = looping;
		}

		void set_align_to_path(bool align) noexcept
		{
			m_align_to_path = align;
		}
	};
}

#endif //GLACIER_PATH_COMPONENT_H_
