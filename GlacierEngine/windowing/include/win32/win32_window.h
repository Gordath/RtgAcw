#ifndef GLACIER_WIN32_WINDOW_H_
#define GLACIER_WIN32_WINDOW_H_

#include "window.h"
#include <map>

namespace Glacier
{
	class Win32Window : public Window {
	private:
		class WindowClass {
		private:
			friend class Win32Window;

			static const std::wstring m_win_class_name;
			static int m_count;

		public:
			WindowClass();
			~WindowClass();
		};

		const WindowClass m_window_class;

		static LRESULT CALLBACK _win_proc_def(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

	protected:
		HWND m_handle;
		HWND m_parent;
		HMENU m_menu;

		unsigned int m_flags{ 0 };
		unsigned int m_flags_ex{ 0 };

	public:
		Win32Window(const std::wstring& title,
		            const Vec2i& size,
		            const Vec2i& position,
		            const unsigned int window_id,
		            const bool focused,
		            const bool minimized,
		            const bool resizeable,
		            const bool show_cursor,
		            const WindowFunctionCallbacks& callbacks);

		virtual LRESULT CALLBACK win_proc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}
#endif //GLACIER_WIN32_WINDOW_H_
