#ifndef GLACIER_ENGINE_CONTEXT_H_
#define GLACIER_ENGINE_CONTEXT_H_

#include "../rendering/include/GAPI_context.h"
#include "../rendering/include/d3d/D3D11_context.h"
#include "../rendering/include/render_system.h"
#include "camera_system.h"
#include "light_system.h"

namespace Glacier
{
	class EngineContext {
	private:
		static GAPIContext* m_GAPI_context;
		static std::unique_ptr<RenderSystem> m_render_system;
		static std::unique_ptr<CameraSystem> m_camera_system;
		static std::unique_ptr<LightSystem> m_light_system;

	public:
		EngineContext() = default;

		EngineContext(const EngineContext& context) = delete;

		EngineContext& operator=(const EngineContext& context) = delete;

		static bool initialize();

#if defined(GLACIERENGINE_BUILD_D3D)
		static D3D11Context* get_GAPI_context() noexcept
		{
			return static_cast<D3D11Context*>(m_GAPI_context);
		}
#endif

		static RenderSystem* get_render_system() noexcept
		{
			return m_render_system.get();
		}

		static CameraSystem* get_camera_system() noexcept
		{
			return m_camera_system.get();
		}

		static LightSystem* get_light_system() noexcept
		{
			return m_light_system.get();
		}
	};
}

#endif //GLACIER_ENGINE_CONTEXT_H_
