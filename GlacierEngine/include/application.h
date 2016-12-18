#ifndef GLACIER_ENGINE_APPLICATION_H_
#define GLACIER_ENGINE_APPLICATION_H_
#include "../windowing/include/windowing_service_locator.h"
#include "internal/engine_context.h"

namespace Glacier
{
	class Application : protected WindowingServiceLocator {
	protected:
		bool _terminate{ false };

		static EngineContext _engine_context;

	public:
		Application() = default;
		Application(const Application& application) = delete;
		Application& operator=(const Application& application) = delete;

		virtual ~Application() = default;

		virtual bool initialize(int* argc, char* argv[]);

		virtual void update() noexcept = 0;

		virtual void draw() const noexcept = 0;

		virtual int  run() noexcept = 0;
	};

#if defined(_MSC_VER)
#  pragma comment(linker, "/ENTRY:mainCRTStartup")
#  if defined(_DEBUG)
#    pragma comment(linker, "/SUBSYSTEM:console ")
#    pragma message(__FILE__ ": /SUBSYSTEM:console /ENTRY:mainCRTStartup")
#  else
#    pragma comment(linker, "/SUBSYSTEM:windows")
#    pragma message(__FILE__ ": /SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#  endif
#endif
}

#endif //GLACIER_ENGINE_APPLICATION_H_
