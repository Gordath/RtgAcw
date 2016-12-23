#ifndef GLACIER_WINDOWING_SERVICE_H_
#define GLACIER_WINDOWING_SERVICE_H_


#include "window.h"
#include <vector>

namespace Glacier
{
	class WindowingService {
	private:
		static std::vector<Window*> m_windows;

	public:
		WindowingService() = default;

		WindowingService(const WindowingService& service) = delete;

		WindowingService& operator=(const WindowingService& service) = delete;

		static void add_window(Window* window);

		static void create(const std::wstring& title,
		            const Vec2i& size,
		            const Vec2i& position,
		            const bool focused,
		            const bool minimized,
		            const bool resizeable,
		            const bool show_cursor,
		            const bool enable_MSAA,
		            const int MSAA_sample_count,
		            const WindowFunctionCallbacks& callbacks);

		//TODO: figure out a nice data structure for traversing and removing windows.
		static void destroy_window(unsigned int win_id);

		static Window* get_window(unsigned int win_id) noexcept;

		static Window* get_window(const std::wstring& title) noexcept;

		static size_t get_window_count() noexcept;

		static void swap_buffers() noexcept;
	};
}

#endif //GLACIER_WINDOWING_SERVICE_H_
