#ifndef GLACIER_WIN32_WINDOW_H_
#define GLACIER_WIN32_WINDOW_H_

#include <windows.h>
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
			GLACIERENGINE_API WindowClass();
			GLACIERENGINE_API ~WindowClass();
		};

		const WindowClass _window_class;

		static std::map<HWND, Window*> _window_by_handle;
		static LRESULT CALLBACK _winProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

	protected:
		HWND  _handle = nullptr;
		HWND  _parent = nullptr;
		HMENU _menu   = nullptr;

		WNDCLASSEX window_class;

		unsigned int _flags    = 0;
		unsigned int _flags_ex = 0;

	public:
		GLACIERENGINE_API Win32Window(const std::string& title,
		            const Vec2i& size,
		            const Vec2i& position,
					const unsigned int window_id,
		            const bool focused,
		            const bool minimized,
		            const bool resizeable,
		            const bool show_cursor);
	};
}
#endif //GLACIER_WIN32_WINDOW_H_
