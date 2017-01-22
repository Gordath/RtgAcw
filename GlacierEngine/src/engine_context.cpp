#include "internal/engine_context.h"
#include "D3D11_context.h"
#include <iostream>
#include "render_state_manager.h"

namespace Glacier
{
#if defined(GLACIERENGINE_BUILD_D3D)
	D3D11Context* EngineContext::m_GAPI_context{ nullptr };
#else
#endif
	std::unique_ptr<RenderSystem> EngineContext::m_render_system{ std::make_unique<RenderSystem>() };
	std::unique_ptr<CameraSystem> EngineContext::m_camera_system{ std::make_unique<CameraSystem>() };
	std::unique_ptr<LightSystem> EngineContext::m_light_system{ std::make_unique<LightSystem>() };

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

		if (!m_render_system->initialize()) {
			return false;
		}

		if (!m_camera_system->initialize()) {
			return false;
		}

		if (!m_light_system->initialize()) {
			return false;
		}

		RenderStateManager::initialize();

		return true;
	}

}
