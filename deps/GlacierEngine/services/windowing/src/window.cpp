#include "../include/window.h"

namespace Glacier
{
	const std::string& Window::get_title() const
	{
		return _title;
	}

	void Window::set_size(const Vec2i &size)
	{
		_size = size;
	}

	const Vec2i& Window::get_size() const
	{
		return _size;
	}

	unsigned Window::get_id() const
	{
		return _win_id;
	}

	const Vec2i& Window::get_mouse_position() const
	{
		return _mouse_pos;
	}

	void Window::set_mouse_position(const Vec2i &mouse_pos)
	{
		_mouse_pos = mouse_pos;
	}

	void Window::set_focus(const bool focus)
	{
		_focused = focus;
	}

	bool Window::is_focused() const
	{
		return _focused;
	}

	void Window::set_minimized(const bool minimized)
	{
		_minimized = minimized;
	}

	bool Window::is_minimized() const
	{
		return _minimized;
	}

	void Window::set_resizable(const bool resizeable)
	{
		_resizeable = resizeable;
	}

	bool Window::is_resizeable() const
	{
		return _resizeable;
	}

	void Window::set_changed_size(const bool state)
	{
		_changed_size = state;
	}

	bool Window::changed_size() const
	{
		return _changed_size;
	}

	void Window::set_redisplay(const bool redisplay)
	{
		_redisplay = redisplay;
	}

	void Window::show_cursor(bool show)
	{
		_show_cursor = show;
	}

	bool Window::must_redisplay() const
	{
		return _redisplay;
	}

	void Window::set_window_callbacks(const WindowFunctionCallbacks& cbs)
	{
		_callbacks = cbs;
	}

	const WindowFunctionCallbacks& Window::get_callbacks() const
	{
		return _callbacks;
	}

}
