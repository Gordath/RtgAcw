#include "observer_subject.h"

namespace Glacier
{
	void ObserverSubject::register_observer(const std::string& msg, Observer* o) noexcept
	{
		auto it{ m_message_listeners.find(msg) };

		if (it == m_message_listeners.end()) {
			m_message_listeners[msg] = std::vector<Observer*>{ o };
		} else {
			m_message_listeners[msg].push_back(o);
		}
	}

	void ObserverSubject::unregister_observer(const std::string& msg, Observer* o) noexcept
	{
		auto map_it{ m_message_listeners.find(msg) };

		if (map_it != m_message_listeners.end()) {
			auto& observers{ map_it->second };

			auto it{ std::find(observers.begin(), observers.end(), o) };
			
			if (it != observers.end()) {
				observers.erase(it);
			}
		}
	}

	void ObserverSubject::broadcast_message(MessageContainer msg) const noexcept
	{
		// Dispatch message to all registered listeners
		auto map_it{ m_message_listeners.find(msg.get()->get_message_type()) };

		// If we have registered listeners for this message
		if (map_it != m_message_listeners.end()) {
			auto& observers{ map_it->second };

			for (auto observer : observers) {
				observer->on_message(msg);
			}
		}
	}
}
