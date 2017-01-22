#ifndef GLACIER_OBJECT_H_
#define GLACIER_OBJECT_H_
#include <vector>
#include "component.h"
#include "internal/types.h"
#include "internal/math_utils.h"
#include "observer_subject.h"

namespace Glacier
{
	class Object : public ObserverSubject {
	private:
		std::vector<Component*> m_components;

		std::string m_name;

		bool m_alive;

		Vec3f m_position;
		Vec3f m_euler_angles;
		Vec3f m_scale{ 1.0f, 1.0f, 1.0f };

		Mat4f m_xform;

		Object* m_parent;

	public:
		explicit Object(const std::string& name) : m_name{ name }, m_alive{true}, m_parent{ nullptr }
		{
		}

		~Object();


		Object(const Object& other)
			: ObserverSubject{other},
			  m_components{other.m_components},
			  m_name{other.m_name},
			  m_alive{other.m_alive},
			  m_position{other.m_position},
			  m_euler_angles{other.m_euler_angles},
			  m_scale{other.m_scale},
			  m_xform{other.m_xform},
			  m_parent{other.m_parent}
		{
		}

		Object& operator=(const Object& other)
		{
			if (this == &other)
				return *this;
			ObserverSubject::operator =(other);
			m_components = other.m_components;
			m_name = other.m_name;
			m_alive = other.m_alive;
			m_position = other.m_position;
			m_euler_angles = other.m_euler_angles;
			m_scale = other.m_scale;
			m_xform = other.m_xform;
			m_parent = other.m_parent;
			return *this;
		}

		const std::string& get_name() const noexcept
		{
			return m_name;
		}

		const Vec3f& get_position() const noexcept
		{
			return m_position;
		}

		void set_position(const Vec3f& position) noexcept
		{
			m_position = position;
		}

		const Vec3f& get_euler_angles() const noexcept
		{
			return m_euler_angles;
		}

		void set_euler_angles(const Vec3f& euler_angles) noexcept
		{
			m_euler_angles = euler_angles;
		}

		const Vec3f& get_scale() const noexcept
		{
			return m_scale;
		}
		
		void set_scale(const Vec3f& scale) noexcept
		{
			m_scale = scale;
		}

		Object* get_parent() const noexcept
		{
			return m_parent;
		}

		void set_parent(Object* object) noexcept
		{
			m_parent = object;
		}

		const Mat4f& get_xform() const noexcept
		{
			return m_xform;
		}

		void set_xform(const Mat4f& xform) noexcept
		{
			m_xform = xform;
		}

		void calculate_xform() noexcept
		{
			m_xform = MathUtils::identity_matrix;

			m_xform = MathUtils::translate(m_xform, m_position);
			m_xform = MathUtils::rotate(m_xform, MathUtils::to_radians(m_euler_angles.y), Vec3f{ 0.0f, 1.0f, 0.0f });
			m_xform = MathUtils::rotate(m_xform, MathUtils::to_radians(m_euler_angles.z), Vec3f{ 0.0f, 0.0f, 1.0f });
			m_xform = MathUtils::rotate(m_xform, MathUtils::to_radians(m_euler_angles.x), Vec3f{1.0f, 0.0f, 0.0f});
			m_xform = MathUtils::scale(m_xform, m_scale);

			if (m_parent) {
				m_xform = m_parent->get_xform() * m_xform;
			}
		}

		Component* get_component(const std::string& type) const noexcept
		{
			for (const auto component : m_components) {
				if (component->get_type() == type) {
					return component;
				}
			}

			return nullptr;
		}

		void add_component(Component* component) noexcept
		{
			m_components.push_back(component);
		}

		bool is_alive() const noexcept
		{
			return m_alive;
		}

		virtual void update(float dt, long time = 0) noexcept
		{
			calculate_xform();

			for (auto component : m_components) {
				component->update(dt, time);
			}
		}

		virtual void setup() noexcept
		{
			for (auto component : m_components) {
				component->setup();
			}
		}

		virtual void teardown() noexcept
		{
			for (auto component : m_components) {
				component->teardown();
			}
		}
	};
}

#endif //GLACIER_OBJECT_H_
