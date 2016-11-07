#ifndef GLACIER_WIN32_WINDOW_H_
#define GLACIER_WIN32_WINDOW_H_

#include "win32utils.h"
//#include <Windows.h>
#include "window.h"
#include <map>

namespace Glacier
{
	class Win32Window : public Window
	{
	private:
		class WindowClass {
		private:
			friend class Win32Window;

			static const std::string _win_class_name;
			int _count = 0;

		public:
			WindowClass();
			~WindowClass();
		};

		const WindowClass _window_class;


		static LRESULT CALLBACK _win_proc_def(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

	protected:
		HWND  _handle = nullptr;
		HWND  _parent = nullptr;
		HMENU _menu   = nullptr;

		WNDCLASSEX window_class;

		unsigned int _flags    = 0;
		unsigned int _flags_ex = 0;

	public:
		Win32Window(const std::string& title,
		            const Vec2i& size,
		            const Vec2i& position,
					const unsigned int window_id,
		            const bool focused,
		            const bool minimized,
		            const bool resizeable,
		            const bool show_cursor);

		virtual LRESULT CALLBACK win_proc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}
#endif //GLACIER_WIN32_WINDOW_H_
