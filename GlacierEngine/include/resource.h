#ifndef GLACIER_RESOURCE_H_
#define GLACIER_RESOURCE_H_
#include <string>

namespace Glacier
{
	class Resource {
	private:
		unsigned int m_id;

	public:
		Resource() : m_id{ 0 }
		{			
		}

		explicit Resource(unsigned int id) : m_id{ id }
		{
		}

		virtual ~Resource();

		unsigned int get_id() const noexcept
		{
			return m_id;
		}

		void set_id(unsigned int id) noexcept
		{
			m_id = id;
		}

		virtual bool load(const std::wstring& file_name) noexcept = 0;
	};
}

#endif //GLACIER_RESOURCE_H_
