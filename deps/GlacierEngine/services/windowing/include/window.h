#ifndef GLACIER_WINDOW_H_
#define GLACIER_WINDOW_H_

#include <string>
#include "dll_export.h"
#include "types.h"
#include "windowing_types.h"

namespace Glacier
{
	class Window {
	protected:
		std::string			_title;
		Vec2i				_size;
		Vec2i				_position;
		unsigned int		_win_id = 0;

		Vec2i				_mouse_pos;

		bool				_focused;
		bool				_minimized;
		bool				_resizeable;
		bool				_show_cursor;

		bool				_changed_size;
		bool				_redisplay;

		WindowFunctionCallbacks _callbacks;
	
	public:
		GLACIERENGINE_API Window(const std::string &title,
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

		GLACIERENGINE_API Window(const Window &win) = delete;

		GLACIERENGINE_API Window &operator=(const Window &win) = delete;

		virtual GLACIERENGINE_API ~Window() = default;

		const std::string GLACIERENGINE_API &get_title() const;

		void GLACIERENGINE_API set_size(const Vec2i &size);

		const Vec2i GLACIERENGINE_API &get_size() const;

		unsigned int GLACIERENGINE_API get_id() const;

		const Vec2i GLACIERENGINE_API &get_mouse_position() const;

		void GLACIERENGINE_API set_mouse_position(const Vec2i &mouse_pos);

		void GLACIERENGINE_API set_focus(const bool focus);

		bool GLACIERENGINE_API is_focused() const;

		void GLACIERENGINE_API set_minimized(const bool minimized);

		bool GLACIERENGINE_API is_minimized() const;

		void GLACIERENGINE_API set_resizable(const bool resizeable);

		bool GLACIERENGINE_API is_resizeable() const;

		void GLACIERENGINE_API set_changed_size(const bool state);

		bool GLACIERENGINE_API changed_size() const;

		void GLACIERENGINE_API set_redisplay(const bool redisplay);

		void GLACIERENGINE_API show_cursor(bool show);

		bool GLACIERENGINE_API must_redisplay() const;

		void GLACIERENGINE_API set_window_callbacks(const WindowFunctionCallbacks &cbs);

		const WindowFunctionCallbacks GLACIERENGINE_API &get_callbacks() const;
	};
}

#endif //GLACIER_WINDOW_H_
