#include "windowing_service_locator.h"

namespace Glacier
{
	WindowingService* WindowingServiceLocator::m_windowing_service{ nullptr };

	void WindowingServiceLocator::provide(WindowingService* service)
	{
		m_windowing_service = service;
	}

	WindowingService* WindowingServiceLocator::get_windowing_service()
	{
		return m_windowing_service;
	}
}
