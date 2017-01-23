#ifndef GLACIER_ENGINE_APPLICATION_H_
#define GLACIER_ENGINE_APPLICATION_H_
#include "internal/engine_context.h"
#include "timer.h"

namespace Glacier
{
	class Application {
	private:
		bool m_terminate{ false };

		static EngineContext m_engine_context;

		Timer m_timer;

	public:
		Application() = default;
		Application(const Application& application) = delete;
		Application& operator=(const Application& application) = delete;

		virtual ~Application();

		void set_termination(bool state) noexcept
		{
			m_terminate = state;
		}

		bool should_terminate() const noexcept
		{
			return m_terminate;
		}

		double get_delta() const noexcept
		{
			return m_timer.get_delta();
		}

		long get_msec() const noexcept
		{
			return m_timer.get_msec();
		}

		double get_sec() const noexcept
		{
			return m_timer.get_sec();
		}

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
