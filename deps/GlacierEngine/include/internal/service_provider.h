#ifndef GLACIER_SERVICE_PROVIDER_H_
#define GLACIER_SERVICE_PROVIDER_H_

#include "service.h"
#include <vector>

namespace Glacier
{
	class ServiceProvider {
	private:
		static std::vector<Service*> _services;

	public:
		static void register_service(Service *service);

		static Service *provide_service(const ServiceType type);
	};
}

#endif //GLACIER_SERVICE_PROVIDER_H_