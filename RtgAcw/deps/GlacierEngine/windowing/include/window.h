#ifndef GLACIER_WINDOW_H_
#define GLACIER_WINDOW_H_

#include <string>
#include "types.h"
#include "windowing_types.h"

namespace Glacier
{
	class Window {
	protected:
		std::string				_title;
		Vec2i					_size;
		Vec2i					_position;
		unsigned int			_win_id = 0;

		Vec2i					_mouse_pos;

		bool					_focused;
		bool					_minimized;
		bool					_resizeable;
		bool					_show_cursor;

		bool					_changed_size;
		bool					_redisplay;

		WindowFunctionCallbacks _callbacks;
	
	public:
		Window(const std::string &title,
			   const Vec2i &size, 
			   const Vec2i &position,
			   const unsigned int window_id,
			   const bool focused,
			   const bool minimized,
			   const bool resizeable,
			   const bool show_cursor) : _title(title),
			                            _size(size),
		                                _position(position),
										_win_id(window_id),
		                                _focused(focused), 
		                                _minimized(minimized),
			                            _resizeable(resizeable),
										_show_cursor(show_cursor)				
		{ }

		Window(const Window &win) = delete;

		Window &operator=(const Window &win) = delete;

		virtual ~Window() = default;

		const std::string &get_title() const;

		void set_size(const Vec2i &size);

		const Vec2i &get_size() const;

		unsigned int get_id() const;

		const Vec2i &get_mouse_position() const;

		void set_mouse_position(const Vec2i &mouse_pos);

		void set_focus(const bool focus);

		bool is_focused() const;

		void set_minimized(const bool minimized);

		bool is_minimized() const;

		void set_resizable(const bool resizeable);

		bool is_resizeable() const;

		void set_changed_size(const bool state);

		bool changed_size() const;

		void set_redisplay(const bool redisplay);

		void show_cursor(bool show);

		bool must_redisplay() const;

		void set_window_callbacks(const WindowFunctionCallbacks &cbs);

		const WindowFunctionCallbacks &get_callbacks() const;
	};
}

#endif //GLACIER_WINDOW_H_
