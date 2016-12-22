#ifndef GLACIER_ENGINE_WINDOWING_SERVICE_LOCATOR_H_
#define GLACIER_ENGINE_WINDOWING_SERVICE_LOCATOR_H_
#include "windowing_service.h"

namespace Glacier
{
	class WindowingServiceLocator {
	private:
		static WindowingService* m_windowing_service;

	protected:
		static WindowingService* get_windowing_service();

	public:
		static void provide(WindowingService* service);

	};
}

#endif //GLACIER_ENGINE_WINDOWING_SERVICE_LOCATOR_H_
