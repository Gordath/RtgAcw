#include "internal/engine_context.h"
#include "D3D11_context.h"
#include <iostream>
#include "GAPI_context_locator.h"
#include "windowing_service_locator.h"

namespace Glacier
{
	GAPIContext* EngineContext::m_GAPI_context{ nullptr };

	WindowingService* EngineContext::m_windowing_service{ nullptr };

	bool EngineContext::initialize()
	{
#if defined(GLACIERENGINE_BUILD_D3D)
		m_GAPI_context = new D3D11Context;
#else
		//Allocate GL context
#endif
		if (!m_GAPI_context->create()) {
			std::cerr << "Failed to initialize the engine's Graphics Context!" << std::endl;
			return false;
		}
		GAPIContextLocator::provide(m_GAPI_context);

		m_windowing_service = new WindowingService;
		WindowingServiceLocator::provide(m_windowing_service);

		return true;
	}
}
