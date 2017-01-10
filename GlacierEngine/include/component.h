#ifndef GLACIER_COMPONENT_H_
#define GLACIER_COMPONENT_H_
#include <string>

namespace Glacier
{
	class Object;

	class Component {
	protected:
		const std::string m_type;

		Object* m_parent{ nullptr };

	public:
		Component(const std::string& type, Object* parent);

		virtual ~Component() = default;

		const std::string& get_type() const noexcept
		{
			return m_type;
		}

		virtual void setup() noexcept = 0;

		virtual void update(float dt, long time = 0) noexcept = 0;

		virtual void teardown() noexcept = 0;
	};
}

#endif //GLACIER_COMPONENT_H_
