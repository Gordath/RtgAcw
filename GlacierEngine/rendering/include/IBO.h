#ifndef GLACIER_ENGINE_IBO_H_
#define GLACIER_ENGINE_IBO_H_
#include <vector>

namespace Glacier
{
	class IBO {
	protected:
		unsigned int m_index_count{ 0 };

	public:
		IBO() = default;

		virtual ~IBO() = default;

		virtual bool create(const std::vector<unsigned int>& indices) noexcept = 0;

		virtual void bind() const noexcept = 0;

		virtual void draw() const noexcept = 0;
	};
}

#endif //GLACIER_ENGINE_IBO_H_
