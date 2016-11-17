#ifndef GLACIER_WINDOWING_SERVICE_H_
#define GLACIER_WINDOWING_SERVICE_H_


#include "window.h"
#include "service.h"
#include <vector>

namespace Glacier
{
	class WindowingService : public Service
	{
	private:
		std::vector<Window*> _windows;

	public:
		WindowingService() = default;

		WindowingService(const WindowingService& service) = delete;

		WindowingService& operator=(const WindowingService& service) = delete;

		void add_window(Window* window);

		void create(const std::string& title,
		            const Vec2i& size,
		            const Vec2i& position,
		            const bool focused,
		            const bool minimized,
		            const bool resizeable,
		            const bool show_cursor,
		            const bool enable_MSAA,
		            const int MSAA_sample_count);

		//TODO: figure out a nice data structure for traversing and removing windows.
		void destroy_window(unsigned int win_id);

		Window* get_window(unsigned int win_id) const;

		Window* get_window(const std::string& title) const;

		size_t get_window_count() const;
	};
}

#endif //GLACIER_WINDOWING_SERVICE_H_
