#include "service_provider.h"
#include <iostream>
#include <cassert>
#include <algorithm>

namespace Glacier
{
	std::vector<Service*> ServiceProvider::_services;

	void ServiceProvider::register_service(Service *service)
	{
		assert(service);

		ServiceType type{ service->get_type() };

		for (const Service *srv : _services) {
			if (srv->get_type() == type) {
				std::cerr << "Service type already exists!" << std::endl;
				return;
			}
		}

		_services.push_back(service);
	}


	Service* ServiceProvider::provide_service(const ServiceType type)
	{
		for(Service *srv : _services) {
			if (srv->get_type() == type) {
				return srv;
			}
		}

		return nullptr;
	}


}
