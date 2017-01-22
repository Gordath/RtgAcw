#ifndef GLACIER_ENGINE_IBO_H_
#define GLACIER_ENGINE_IBO_H_
#include <vector>

namespace Glacier
{
	class IBO {
	private:
		unsigned int m_index_count{ 0 };

	public:
		IBO() = default;

		virtual ~IBO() = default;

		void set_index_count(unsigned int idx_count) noexcept
		{
			m_index_count = idx_count;
		}

		unsigned int get_index_count() const noexcept
		{
			return m_index_count;
		}

		virtual bool create(const std::vector<unsigned int>& indices) noexcept = 0;

		virtual void bind() const noexcept = 0;

		virtual void draw() const noexcept = 0;
	};
}

#endif //GLACIER_ENGINE_IBO_H_
