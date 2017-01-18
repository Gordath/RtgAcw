#ifndef GLACIER_TYPES_H_
#define GLACIER_TYPES_H_

#include "GLM/glm.hpp"
#include "GLM/gtc/constants.inl"
#include "wrl.h"
#include "message.h"
#include <iostream>

namespace Glacier
{
	using Vec2i = glm::ivec2;
	using Vec3i = glm::ivec3;
	using Vec4i = glm::ivec4;
	
	using Vec2ui = glm::uvec2;
	using Vec3ui = glm::uvec3;
	using Vec4ui = glm::uvec4;

	using Vec2f = glm::vec2;
	using Vec3f = glm::vec3;
	using Vec4f = glm::vec4;

	using Vec2d = glm::dvec2;
	using Vec3d = glm::dvec3;
	using Vec4d = glm::dvec4;

	using Mat2f = glm::mat2;
	using Mat3f = glm::mat3;
	using Mat4f = glm::mat4;

	using Mat2d = glm::dmat2;
	using Mat3d = glm::dmat3;
	using Mat4d = glm::dmat4;

	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	template<typename T>
	class RefCountedContainer {
	private:
		T* m_data{ nullptr };
		int m_reference_count{ 0 };

	public:
		RefCountedContainer(T* item)
		{
			m_data = item;
			add_reference();
		}

		~RefCountedContainer()
		{
			subtract_reference();

			if (!m_reference_count) {
				delete m_data;
			}
		}


		RefCountedContainer(const RefCountedContainer& other)
			: m_data(other.m_data),
			  m_reference_count(other.m_reference_count)
		{
			add_reference();
		}

		RefCountedContainer(RefCountedContainer&& other) noexcept = delete;

		RefCountedContainer& operator=(const RefCountedContainer& other)
		{
			if (this == &other) {
				return *this;
			}

			subtract_reference();

			if (!m_reference_count) {
				delete m_data;
			}

			m_data = other.m_data;
			m_reference_count = other.m_reference_count;

			add_reference();

			return *this;
		}

		RefCountedContainer& operator=(RefCountedContainer&& other) noexcept = delete;

		void add_reference() noexcept
		{
			++m_reference_count;
		}

		void subtract_reference() noexcept
		{
			--m_reference_count;
		}

		int get_reference_count() const noexcept
		{
			return m_reference_count;
		}

		T* get() const noexcept
		{
			return m_data;
		}
	};

	using MessageContainer = RefCountedContainer<Message>;
}


#endif //GLACIER_TYPES_H_
