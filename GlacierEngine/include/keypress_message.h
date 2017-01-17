#ifndef GLACIER_KEYPRESS_MESSAGE_H_
#define GLACIER_KEYPRESS_MESSAGE_H_
#include "message.h"

namespace Glacier
{
	class KeypressMessage : public Message {
	private:
		unsigned char m_key;
		int m_x;
		int m_y;
		bool m_pressed;

	public:
		KeypressMessage(unsigned char key, int x, int y, bool pressed)
			: Message{ "msg_keypress" },
			  m_key{ key },
			  m_x{ x },
			  m_y{ y },
			  m_pressed{ pressed }
		{
		}

		unsigned char get_key() const noexcept 
		{
			return m_key;
		}

		int get_x() const noexcept
		{
			return m_x;
		}

		int get_y() const noexcept
		{
			return m_y;
		}

		bool is_pressed() const noexcept
		{
			return m_pressed;
		}
	};
}

#endif //GLACIER_KEYPRESS_MESSAGE_H_
