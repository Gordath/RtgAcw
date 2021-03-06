#include "win32_window.h"
#include <sstream>
#include <iostream>
#include "win32_utils.h"
#include <AntTweakBar.h>

namespace Glacier
{
	const std::wstring Win32Window::WindowClass::m_win_class_name{ L"GlacierWindowClass" };
	int Win32Window::WindowClass::m_count{ 0 };

	// Private Class implementation ----------------------------------------------------------------
	Win32Window::WindowClass::WindowClass()
	{
		if (m_count <= 0) {
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
			wc.lpszClassName = m_win_class_name.c_str();

			if (!::RegisterClassEx(&wc)) {
				std::ostringstream ostr;

				ostr << "Unable to register window class '" << m_win_class_name.c_str() << "'";

				throw std::runtime_error(ostr.str().c_str());
			}
		}

		++m_count;
	}

	Win32Window::WindowClass::~WindowClass()
	{
		--m_count;

		if (m_count >= 0) {
			UnregisterClass(m_win_class_name.c_str(), ::GetModuleHandle(nullptr));
		}
	}

	// ---------------------------------------------------------------------------------------------


	Win32Window::Win32Window(const std::wstring& title,
	                         const Vec2i& size,
	                         const Vec2i& position,
	                         const unsigned int window_id,
	                         const bool focused,
	                         const bool minimized,
	                         const bool resizeable,
	                         const bool show_cursor,
	                         const WindowFunctionCallbacks& callbacks) : Window(title,
	                                                                            size,
	                                                                            position,
	                                                                            window_id,
	                                                                            focused,
	                                                                            minimized,
	                                                                            resizeable,
	                                                                            show_cursor,
	                                                                            callbacks),
																			m_handle{nullptr},
																			m_parent{ nullptr },
																			m_menu{nullptr}
																			
	{
		RECT wr = { 0, 0, size.x, size.y };

		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

		const unsigned int window_flags_enable{
			WS_CLIPCHILDREN |
			WS_CLIPSIBLINGS |
			WS_OVERLAPPEDWINDOW |
			WS_VISIBLE
		};

		const unsigned int window_flags_disable{ WS_DLGFRAME };
		const unsigned int window_flags_ex_enable{ WS_EX_CLIENTEDGE };
		const unsigned int window_flags_ex_disable{ 0 };

		m_flags = window_flags_enable & ~window_flags_disable;
		m_flags_ex = window_flags_ex_enable & ~window_flags_ex_disable;

		m_handle = ::CreateWindowEx(m_flags_ex,
		                            WindowClass::get_win_class_name().c_str(),
		                            title.c_str(),
		                            m_flags,
		                            position.x,
		                            position.y,
		                            size.x,
		                            size.y,
		                            m_parent,
		                            m_menu,
		                            ::GetModuleHandle(nullptr),
		                            this); //Carry the window class pointer.

//		if (!m_handle) {
//			char buff[256];
//			snprintf(buff, 256, "Error creating window with name: '%s'", title);
//
//			throw std::runtime_error(buff);
//		}

		ShowWindow(m_handle, SW_SHOWDEFAULT);
		UpdateWindow(m_handle);
		ShowCursor(Window::show_cursor());
	}

	LRESULT CALLBACK Win32Window::win_proc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
	{

		if (TwEventWin(handle, msg, wparam, lparam))
			return 0; // Event has been handled by AntTweakBar

		auto callbacks{ get_callbacks() };
		switch (msg) {
			
		case WM_PAINT:
			ValidateRect(handle, nullptr);
			break;
		case WM_CLOSE:
			break;
		case WM_SIZE:
			set_size(Vec2i{ LOWORD(lparam), HIWORD(lparam) });
			if (callbacks.reshape_func) {
				callbacks.reshape_func(get_size().x, get_size().y);
			}
			break;
		case WM_KEYDOWN:
			if (wparam < 256) {
				if (callbacks.keyboard_func) {
					callbacks.keyboard_func(wparam, get_mouse_position().x, get_mouse_position().y);
				}
			} else {
				if (callbacks.special_func) {
					callbacks.special_func(wparam, get_mouse_position().x, get_mouse_position().y);
				}
			}
			break;
		case WM_KEYUP:
			if (wparam < 256) {
				if (callbacks.keyboard_up_func) {
					callbacks.keyboard_up_func(wparam, get_mouse_position().x, get_mouse_position().y);
				}
			} else {
				if (callbacks.special_up_func) {
					callbacks.special_up_func(wparam, get_mouse_position().x, get_mouse_position().y);
				}
			}
			break;
		case WM_MOUSEMOVE:
			set_mouse_position(Vec2i{ LOWORD(lparam), HIWORD(lparam) });
			if (wparam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)) {
				if (callbacks.motion_func) {
					callbacks.motion_func(get_mouse_position().x, get_mouse_position().y);
				}
			} else {
				if (callbacks.passive_motion_func) {
					callbacks.passive_motion_func(get_mouse_position().x, get_mouse_position().y);
				}
			}
			break;
		case WM_LBUTTONDOWN:
			if (callbacks.mouse_func) {
				callbacks.mouse_func(0, true, LOWORD(lparam), HIWORD(lparam));
			}
			break;
		case WM_RBUTTONDOWN:
			if (callbacks.mouse_func) {
				callbacks.mouse_func(2, true, LOWORD(lparam), HIWORD(lparam));
			}
			break;
		case WM_MBUTTONDOWN:
			if (callbacks.mouse_func) {
				callbacks.mouse_func(1, true, LOWORD(lparam), HIWORD(lparam));
			}
			break;
		case WM_LBUTTONUP:
			if (callbacks.mouse_func) {
				callbacks.mouse_func(0, false, LOWORD(lparam), HIWORD(lparam));
			}
			break;
		case WM_RBUTTONUP:
			if (callbacks.mouse_func) {
				callbacks.mouse_func(2, false, LOWORD(lparam), HIWORD(lparam));
			}
			break;
		case WM_MBUTTONUP:
			if (callbacks.mouse_func) {
				callbacks.mouse_func(1, false, LOWORD(lparam), HIWORD(lparam));
			}
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

		Win32Window* win{ nullptr };
		//get the window by it's handle
		win = get_window_long_ptr<Win32Window>(handle, GWL_USERDATA);
		if (win) {
			//execute the window specific win_proc.
			return win->win_proc(handle, msg, wparam, lparam);
		}

		return DefWindowProc(handle, msg, wparam, lparam);
	}
}
