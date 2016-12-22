#ifndef GLACIER_ENGINE_CONTEXT_H_
#define GLACIER_ENGINE_CONTEXT_H_

#include "../rendering/include/GAPI_context.h"
#include "../windowing/include/windowing_service.h"

namespace Glacier
{
	class EngineContext {
	private:
		static GAPIContext* m_GAPI_context;
		static WindowingService* m_windowing_service;

	public:
		EngineContext() = default;

		EngineContext(const EngineContext& context) = delete;

		EngineContext& operator=(const EngineContext& context) = delete;

		static bool initialize();
	};
}

#endif //GLACIER_ENGINE_CONTEXT_H_
