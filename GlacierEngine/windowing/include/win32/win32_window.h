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
			//friend class Win32Window;

			static const std::wstring m_win_class_name;
			static int m_count;

		public:
			WindowClass();
			~WindowClass();

			WindowClass(const WindowClass&) = delete;

			WindowClass& operator=(const WindowClass&) = delete;

			static const std::wstring& get_win_class_name()
			{
				return m_win_class_name;
			}
		};

		const WindowClass m_window_class;

		static LRESULT CALLBACK _win_proc_def(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

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

		Win32Window(const Win32Window&) = delete;

		Win32Window& operator=(const Win32Window&) = delete;

		HWND get_handle() const
		{
			return m_handle;
		}

		void set_handle(HWND hwnd)
		{
			m_handle = hwnd;
		}

		HWND get_parent() const
		{
			return m_parent;
		}

		void set_parent(HWND hwnd)
		{
			m_parent = hwnd;
		}

		HMENU get_menu() const
		{
			return m_menu;
		}

		void set_menu(HMENU hmenu)
		{
			m_menu = hmenu;
		}

		unsigned int get_flags() const
		{
			return m_flags;
		}

		void set_flags(unsigned int flags)
		{
			m_flags = flags;
		}

		unsigned int get_flags_ex() const
		{
			return m_flags_ex;
		}

		void set_flags_ex(unsigned int flags_ex)
		{
			m_flags_ex = flags_ex;
		}
	};
}
#endif //GLACIER_WIN32_WINDOW_H_
