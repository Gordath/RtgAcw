#include "../include/win32window.h"
#include <sstream>

namespace Glacier
{
	const std::string Win32Window::WindowClass::_win_class_name{"GlacierWindowClass"};
	std::map<HWND, Window*> Win32Window::_window_by_handle;

	// Private Class implementation ----------------------------------------------------------------
	Win32Window::WindowClass::WindowClass()
	{
		WNDCLASSEX wc = {0};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = ::GetModuleHandle(nullptr);
		wc.style = (CS_HREDRAW | CS_VREDRAW);
		wc.lpfnWndProc = _winProc;
		wc.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
		wc.hIconSm = ::LoadIcon(nullptr, IDI_APPLICATION);
		wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = _win_class_name.c_str();

		if (!::RegisterClassEx(&wc)) {
			std::ostringstream ostr;

			ostr << "Unable to register window class '" << _win_class_name.c_str() << "'";

			throw std::runtime_error(ostr.str().c_str());
		}

		++_count;
	}

	Win32Window::WindowClass::~WindowClass()
	{
		--_count;

		if (_count >= 0) {
			::UnregisterClass(_win_class_name.c_str(), ::GetModuleHandle(nullptr));
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

		_flags = (window_flags_enable    & ~window_flags_disable);
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
		                           nullptr);

		if (!_handle) {
			char buff[256];
			snprintf(buff, 256, "Error creating window with name: '%s'", title);

			throw std::runtime_error(buff);
		}

		_window_by_handle[_handle] = this;

		ShowWindow(_handle, SW_SHOWDEFAULT);
		UpdateWindow(_handle);
		ShowCursor(_show_cursor);
	}
	
	LRESULT CALLBACK Win32Window::_winProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		Window* win{_window_by_handle[handle]};

		if(!win) {
			return DefWindowProc(handle, msg, wparam, lparam);
		}

		const WindowFunctionCallbacks& callbacks{win->get_callbacks()};

		Vec2i mouse_pos{win->get_mouse_position()};

		switch (msg) {
		case WM_PAINT:
			win->set_redisplay(true);
			ValidateRect(handle, nullptr);
			break;

		case WM_CLOSE:

			break;
		case WM_SIZE:
			win->set_size(Vec2i{LOWORD(lparam), HIWORD(lparam)});
			win->set_changed_size(true);
			break;

		case WM_KEYDOWN:
			if (wparam < 256) {
				if (callbacks._keyboard_func) {
					callbacks._keyboard_func(wparam, mouse_pos.x, mouse_pos.y);
				}
			}
			else {
				if (callbacks._special_func) {
					callbacks._special_func(wparam, mouse_pos.x, mouse_pos.y);
				}
			}
			break;

		case WM_KEYUP:
			if (wparam < 256) {
				if (callbacks._keyboard_up_func) {
					callbacks._keyboard_up_func(wparam, mouse_pos.x, mouse_pos.y);
				}
			}
			else {
				if (callbacks._special_up_func) {
					callbacks._special_up_func(wparam, mouse_pos.x, mouse_pos.y);
				}
			}
			break;

		case WM_MOUSEMOVE:
			win->set_mouse_position(Vec2i{LOWORD(lparam), HIWORD(lparam)});

			if (wparam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)) {
				if (callbacks._motion_func) {
					callbacks._motion_func(mouse_pos.x, mouse_pos.y);
				}
			}
			else {
				if (callbacks._passive_motion_func) {
					callbacks._passive_motion_func(mouse_pos.x, mouse_pos.y);
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
			break;
		default:
			return DefWindowProc(handle, msg, wparam, lparam);
		}

		return 0;
	}
}
