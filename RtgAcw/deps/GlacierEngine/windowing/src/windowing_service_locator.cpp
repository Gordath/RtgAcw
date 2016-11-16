#include "windowing_service_locator.h"

namespace Glacier
{
	WindowingService* WindowingServiceLocator::_windowing_service;

	void WindowingServiceLocator::provide(WindowingService* service)
	{
		_windowing_service = service;
	}

	WindowingService* WindowingServiceLocator::get_windowing_service()
	{
		return _windowing_service;
	}
}
