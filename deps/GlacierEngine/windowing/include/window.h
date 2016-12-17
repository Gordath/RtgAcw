#ifndef GLACIER_WINDOW_H_
#define GLACIER_WINDOW_H_

#include <string>
#include "types.h"
#include "windowing_types.h"

namespace Glacier
{
	class Window {
	protected:
		std::string m_title;
		Vec2i m_size;
		Vec2i m_position;
		unsigned int m_win_id = 0;

		Vec2i m_mouse_pos;

		bool m_focused;
		bool m_minimized;
		bool m_resizeable;
		bool m_show_cursor;

		bool m_changed_size;
		bool m_redisplay;

		WindowFunctionCallbacks m_callbacks;

	public:
		Window(const std::string& title,
		       const Vec2i& size,
		       const Vec2i& position,
		       const unsigned int window_id,
		       const bool focused,
		       const bool minimized,
		       const bool resizeable,
		       const bool show_cursor) : m_title(title),
		                                 m_size(size),
		                                 m_position(position),
		                                 m_win_id(window_id),
		                                 m_focused(focused),
		                                 m_minimized(minimized),
		                                 m_resizeable(resizeable),
		                                 m_show_cursor(show_cursor)
		{
		}

		Window(const Window& win) = delete;

		Window& operator=(const Window& win) = delete;

		virtual ~Window() = default;

		const std::string& get_title() const noexcept { return m_title; }

		void set_size(const Vec2i& size) noexcept { m_size = size; }

		const Vec2i& get_size() const noexcept { return m_size; }

		unsigned int get_id() const noexcept { return m_win_id; }

		const Vec2i& get_mouse_position() const noexcept { return m_mouse_pos; }

		void set_mouse_position(const Vec2i& mouse_pos) noexcept { m_mouse_pos = mouse_pos; }

		void set_focus(const bool focus) noexcept { m_focused = focus; }

		bool is_focused() const noexcept { return m_focused; }

		void set_minimized(const bool minimized) noexcept { m_minimized = minimized; }

		bool is_minimized() const noexcept { return m_minimized; }

		void set_resizable(const bool resizeable) noexcept { m_resizeable = resizeable; }

		bool is_resizeable() const noexcept { return m_resizeable; }

		void set_changed_size(const bool state) noexcept { m_changed_size = state; }

		bool changed_size() const noexcept { return m_changed_size; }

		void set_redisplay(const bool redisplay) noexcept { m_redisplay = redisplay; }

		void show_cursor(bool show) noexcept { m_show_cursor = show; }

		void set_window_callbacks(const WindowFunctionCallbacks& callbacks) noexcept { m_callbacks = callbacks; }

		const WindowFunctionCallbacks& get_callbacks() const noexcept { return m_callbacks; }
	};
}

#endif //GLACIER_WINDOW_H_
