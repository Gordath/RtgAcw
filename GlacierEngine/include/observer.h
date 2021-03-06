#ifndef GLACIER_OBSERVER_H_
#define GLACIER_OBSERVER_H_
#include "message.h"
#include "internal/types.h"

namespace Glacier
{
	class Observer {
	public:
		Observer() = default;
		virtual ~Observer();

		/// Receive a message (called when notified of a message by the subject)
		virtual void on_message(const MessageContainer& msg) = 0;
	};
}

#endif //GLACIER_OBSERVER_H_
