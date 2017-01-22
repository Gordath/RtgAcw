#ifndef GLACIER_COMPONENT_H_
#define GLACIER_COMPONENT_H_
#include <string>
#include "observer.h"

namespace Glacier
{
	class Object;

	class Component : public Observer {
	private:
		const std::string m_type;

		Object* m_parent{ nullptr };

	public:
		Component(const std::string& type, Object* parent);

		Component(const Component& other) = default;
		explicit Component(Component&& other) noexcept = default;
		Component& operator=(const Component& other) = default;
		Component& operator=(Component&& other) noexcept = default;

		virtual ~Component() = default;

		const std::string& get_type() const noexcept
		{
			return m_type;
		}

		Object* get_parent() const noexcept
		{
			return m_parent;
		}

		virtual void setup() noexcept = 0;

		virtual void update(float dt, long time = 0) noexcept = 0;

		virtual void teardown() noexcept = 0;

		void on_message(MessageContainer msg) override;
	};
}

#endif //GLACIER_COMPONENT_H_
