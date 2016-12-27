#include "internal/engine_context.h"
#include "D3D11_context.h"
#include <iostream>

namespace Glacier
{
	GAPIContext* EngineContext::m_GAPI_context{ nullptr };
	std::unique_ptr<RenderSystem> EngineContext::m_render_system{ std::make_unique<RenderSystem>() };

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

		return true;
	}

#if defined(GLACIERENGINE_BUILD_D3D)
	D3D11Context* EngineContext::get_GAPI_context() noexcept
	{
		return static_cast<D3D11Context*>(m_GAPI_context);
	}
#endif
}
