#include "win32_window.h"
#include <sstream>
#include <iostream>

namespace Glacier
{
	const std::string Win32Window::WindowClass::_win_class_name{"GlacierWindowClass"};
	int Win32Window::WindowClass::_count{ 0 };

	// Private Class implementation ----------------------------------------------------------------
	Win32Window::WindowClass::WindowClass()
	{
		if (_count <= 0) {
			WNDCLASSEX wc = { 0 };
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = GetModuleHandle(nullptr);
			wc.style = (CS_HREDRAW | CS_VREDRAW);
			wc.lpfnWndProc = _win_proc_def;
			wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
			wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
			wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
			wc.lpszMenuName = nullptr;
			wc.lpszClassName = _win_class_name.c_str();

			if (!::RegisterClassEx(&wc)) {
				std::ostringstream ostr;

				ostr << "Unable to register window class '" << _win_class_name.c_str() << "'";

				throw std::runtime_error(ostr.str().c_str());
			}
		}

		++_count;
	}

	Win32Window::WindowClass::~WindowClass()
	{
		--_count;

		if (_count >= 0) {
			UnregisterClass(_win_class_name.c_str(), ::GetModuleHandle(nullptr));
		}
	}

	// ---------------------------------------------------------------------------------------------


	Win32Window::Win32Window(const std::string& title,
	                         const Vec2i& size,
	                         const Vec2i& position,
	                         const unsigned int window_id,
	                         const bool focused,
	                         const bool minimized,
	                         const bool resizeable,
	                         const bool show_cursor) : Window(title, size, position, window_id,
	                                                          focused, minimized, resizeable, show_cursor)
	{
		RECT wr = {0, 0, size.x, size.y};

		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

		unsigned const window_flags_enable(WS_CLIPCHILDREN |
			WS_CLIPSIBLINGS |
			WS_OVERLAPPEDWINDOW |
			WS_VISIBLE);
		unsigned const window_flags_disable(WS_DLGFRAME);
		unsigned const window_flags_ex_enable(WS_EX_CLIENTEDGE);
		unsigned const window_flags_ex_disable(0);

		_flags = (window_flags_enable & ~window_flags_disable);
		_flags_ex = (window_flags_ex_enable & ~window_flags_ex_disable);

		_handle = ::CreateWindowEx(_flags_ex,
		                           WindowClass::_win_class_name.c_str(),
		                           title.c_str(),
		                           _flags,
		                           position.x,
		                           position.y,
		                           size.x,
		                           size.y,
		                           _parent,
		                           _menu,
		                           ::GetModuleHandle(nullptr),
		                           this); //Carry the window class pointer.

		if (!_handle) {
			char buff[256];
			snprintf(buff, 256, "Error creating window with name: '%s'", title);

			throw std::runtime_error(buff);
		}

		ShowWindow(_handle, SW_SHOWDEFAULT);
		UpdateWindow(_handle);
		ShowCursor(m_show_cursor);
	}

	LRESULT CALLBACK Win32Window::win_proc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg) {
		case WM_PAINT:
			set_redisplay(true);
			ValidateRect(handle, nullptr);
			break;
		case WM_CLOSE:
			break;
		case WM_SIZE:
			set_size(Vec2i{LOWORD(lparam), HIWORD(lparam)});
			set_changed_size(true);
			break;
		case WM_KEYDOWN:
			std::cout << "WindowID: " << m_win_id << " Keydown!!" << std::endl;
			if (wparam < 256) {
				if (m_callbacks._keyboard_func) {
					m_callbacks._keyboard_func(wparam, m_mouse_pos.x, m_mouse_pos.y);
				}
			}
			else {
				if (m_callbacks._special_func) {
					m_callbacks._special_func(wparam, m_mouse_pos.x, m_mouse_pos.y);
				}
			}
			break;
		case WM_KEYUP:
			if (wparam < 256) {
				if (m_callbacks._keyboard_up_func) {
					m_callbacks._keyboard_up_func(wparam, m_mouse_pos.x, m_mouse_pos.y);
				}
			}
			else {
				if (m_callbacks._special_up_func) {
					m_callbacks._special_up_func(wparam, m_mouse_pos.x, m_mouse_pos.y);
				}
			}
			break;
		case WM_MOUSEMOVE:
			set_mouse_position(Vec2i{LOWORD(lparam), HIWORD(lparam)});

			if (wparam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)) {
				if (m_callbacks._motion_func) {
					m_callbacks._motion_func(m_mouse_pos.x, m_mouse_pos.y);
				}
			}
			else {
				if (m_callbacks._passive_motion_func) {
					m_callbacks._passive_motion_func(m_mouse_pos.x, m_mouse_pos.y);
				}
			}
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_MBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			break;
		case WM_RBUTTONUP:
			break;
		case WM_MBUTTONUP:
			break;
		case WM_MOUSEWHEEL:
			//int delta = GET_WHEEL_DELTA_WPARAM(wparam);
			break;
		case WM_DESTROY:
			PostQuitMessage(WM_QUIT);
			break;
		default:
			break;
		}

		return DefWindowProc(handle, msg, wparam, lparam);
	}


	LRESULT CALLBACK Win32Window::_win_proc_def(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg) {
		case WM_NCCREATE:
		case WM_NCDESTROY: {
			Win32Window* win{ nullptr };
			if (msg == WM_NCCREATE) {
				//extract the Win32Window we stored in the window creation from the lparam.
				win = static_cast<Win32Window*>(reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);
			}

			//map the window with it's handle.
			set_window_long_ptr(handle, GWL_USERDATA, win);

			return DefWindowProc(handle, msg, wparam, lparam);
		}
		default:
			break;
		}

		Win32Window* win{nullptr};
		//get the window by it's handle
		win = get_window_long_ptr<Win32Window>(handle, GWL_USERDATA);
		if (win) {
			//execute the window specific win_proc.
			return win->win_proc(handle, msg, wparam, lparam);
		}

		return DefWindowProc(handle, msg, wparam, lparam);
	}
}
