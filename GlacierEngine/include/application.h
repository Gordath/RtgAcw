#ifndef GLACIER_ENGINE_APPLICATION_H_
#define GLACIER_ENGINE_APPLICATION_H_
#include "internal/engine_context.h"
#include "timer.h"

namespace Glacier
{
	class Application {
	protected:
		bool m_terminate{ false };

		static EngineContext m_engine_context;

		Timer m_timer;

	public:
		Application() = default;
		Application(const Application& application) = delete;
		Application& operator=(const Application& application) = delete;

		virtual ~Application() = default;

		virtual bool initialize(int* argc, char* argv[]);

		virtual void update() noexcept = 0;

		virtual void draw() const noexcept = 0;

		virtual int run() noexcept = 0;
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
