#ifndef GLACIER_MESSAGE_H_
#define GLACIER_MESSAGE_H_
#include <string>

namespace Glacier
{
	class Message {
	private:
		std::string m_type;

	public:
		explicit Message(const std::string& type) : m_type{ type }
		{
		}

		virtual ~Message() = default;

		std::string get_message_type() const noexcept 
		{ 
			return m_type; 
		}
	};
}

#endif //GLACIER_MESSAGE_H_