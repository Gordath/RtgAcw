#ifndef GLACIER_ENGINE_CONTEXT_H_
#define GLACIER_ENGINE_CONTEXT_H_

#include "../rendering/include/GAPI_context.h"
#include "../rendering/include/d3d/D3D11_context.h"

namespace Glacier
{
	class EngineContext {
	private:
		static GAPIContext* m_GAPI_context;

	public:
		EngineContext() = default;

		EngineContext(const EngineContext& context) = delete;

		EngineContext& operator=(const EngineContext& context) = delete;

		static bool initialize();

#if defined(GLACIERENGINE_BUILD_D3D)
		static D3D11Context* get_GAPI_context() noexcept;
#endif

	};
}

#endif //GLACIER_ENGINE_CONTEXT_H_
