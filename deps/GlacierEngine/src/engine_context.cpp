#include "engine_context.h"
#include "D3D11_context.h"
#include <iostream>
#include "GAPI_context_locator.h"
#include "windowing_service_locator.h"

namespace Glacier
{
	GAPIContext* EngineContext::_GAPI_context;

	WindowingService* EngineContext::_windowing_service;

	bool EngineContext::initialize()
	{
#if defined(GLACIERENGINE_BUILD_D3D)
		_GAPI_context = new D3D11Context;
#else
		//Allocate GL context
#endif
		if(!_GAPI_context->create()) {
			std::cerr << "Failed to initialize the engine's Graphics Context!" << std::endl;
			return false;
		}
		GAPIContextLocator::provide(_GAPI_context);

		_windowing_service = new WindowingService;
		WindowingServiceLocator::provide(_windowing_service);

		return true;
	}

}
