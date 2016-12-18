#include "windowing_service.h"
#include "D3D11_window.h"
#include <algorithm>

namespace Glacier
{
	void WindowingService::add_window(class Window* window)
	{
		m_windows.push_back(window);
	}

	void WindowingService::create(const std::wstring& title,
	                              const Vec2i& size,
	                              const Vec2i& position,
	                              const bool focused,
	                              const bool minimized,
	                              const bool resizeable,
	                              const bool show_cursor,
	                              const bool enable_MSAA,
	                              const int MSAA_sample_count,
	                              const WindowFunctionCallbacks& callbacks)
	{
		Window* window;
#if defined(WIN32)
#if defined(GLACIERENGINE_BUILD_D3D)
		window = new D3D11Window(title,
		                         size,
		                         position,
		                         m_windows.size(),
		                         focused,
		                         minimized,
		                         resizeable,
		                         show_cursor,
		                         enable_MSAA,
		                         MSAA_sample_count,
		                         callbacks);
#elif defined(GLACIERENGINE_BUILD_OPENGL)
		//Create win32 GL window.
#endif
#elif defined(__APPLE__)
		//create Apple GL window.
#else
		//create Linux GL window.
#endif

		add_window(window);
	}


	void WindowingService::destroy_window(unsigned int win_id)
	{
		auto it{ std::find_if(m_windows.begin(), m_windows.end(),
		                      [win_id](Window* win) -> bool {
			                      return win_id == win->get_id();
		                      }) };
		delete *it;
		m_windows.erase(it);
	}

	Window* WindowingService::get_window(unsigned int win_id) const noexcept
	{
		for (Window* win : m_windows) {
			if (win->get_id() == win_id) {
				return win;
			}
		}

		return nullptr;
	}

	Window* WindowingService::get_window(const std::wstring& title) const noexcept
	{
		for (Window* win : m_windows) {
			if (win->get_title() == title) {
				return win;
			}
		}

		return nullptr;
	}

	size_t WindowingService::get_window_count() const noexcept
	{
		return m_windows.size();
	}


	void WindowingService::swap_buffers() const noexcept
	{
		for (const auto window : m_windows) {
			window->swap_buffers();
		}
	}
}
