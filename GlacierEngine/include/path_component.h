#ifndef GLACIER_PATH_COMPONENT_H_
#define GLACIER_PATH_COMPONENT_H_
#include "component.h"
#include <vector>

namespace Glacier
{
	class PathComponent : public Component {
	private:
		std::vector<Vec4f> m_points;

	public:
		PathComponent(Object* parent) : Component{ "co_path", parent }
		{
		}

		void setup() noexcept override;

		void update(float dt, long time) noexcept override;

		void teardown() noexcept override;

		void add_point(const Vec4f& point) noexcept
		{
			m_points.push_back(point);
		}
	};
}

#endif //GLACIER_PATH_COMPONENT_H_
