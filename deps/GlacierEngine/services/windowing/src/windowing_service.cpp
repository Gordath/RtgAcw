#include "../include/windowing_service.h"

namespace Glacier
{
	void WindowingService::add(class Window* window)
	{
		_windows.push_back(window);
	}

	void WindowingService::remove()
	{
		
	}

	Window* WindowingService::get_window(unsigned int win_id) const
	{
		for (Window *win : _windows) {
			if (win->get_id() == win_id) {
				return win;
			}
		}

		return nullptr;
	}

	Window* WindowingService::get_window(const std::string& title) const
	{
		for (Window *win : _windows) {
			if(win->get_title() == title) {
				return win;
			}
		}

		return nullptr;
	}

	size_t WindowingService::get_window_count() const
	{
		return _windows.size();
	}

}
