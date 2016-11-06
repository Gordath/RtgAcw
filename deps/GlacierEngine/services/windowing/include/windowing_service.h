#ifndef GLACIER_WINDOWING_SERVICE_H_
#define GLACIER_WINDOWING_SERVICE_H_


#include "window.h"
#include "service.h"
#include <vector>
#include <map>

namespace Glacier
{
	class WindowingService : public Service {
	private:
		std::vector<Window*> _windows;

	public:
		WindowingService() : Service(ServiceType::SRV_WINDOWING)
		{ }

		WindowingService(const WindowingService &service) = delete;

		WindowingService &operator=(const WindowingService &service) = delete;

		void GLACIERENGINE_API add(Window *window);

		//TODO: figure out a nice data structure for traversing and removing windows.
		void GLACIERENGINE_API remove();

		Window GLACIERENGINE_API *get_window(unsigned int win_id) const;

		Window GLACIERENGINE_API *get_window(const std::string &title) const;

		size_t GLACIERENGINE_API get_window_count() const;
	};
}

#endif //GLACIER_WINDOWING_SERVICE_H_