#ifndef GLACIER_OBSERVER_SUBJECT_H_
#define GLACIER_OBSERVER_SUBJECT_H_
#include <string>
#include "observer.h"
#include <vector>
#include <map>
#include "internal/types.h"

namespace Glacier
{
	class ObserverSubject {
	protected:
		// Listener registration
		std::map<std::string, std::vector<Observer*>> m_message_listeners;

	public:
		virtual ~ObserverSubject() = default;

		// Register an observer for a particular type of message
		void register_observer(const std::string& msg, Observer* o) noexcept;

		// De-register an observer
		void unregister_observer(const std::string& msg, Observer* o) noexcept;

		// Broadcast a message to any listeners
		void broadcast_message(MessageContainer msg) const noexcept;
	};
}

#endif //GLACIER_OBSERVER_SUBJECT_H_
