#ifndef GLACIER_ENGINE_APPLICATION_H_
#define GLACIER_ENGINE_APPLICATION_H_
#include "../rendering/include/GAPI_context.h"

namespace Glacier
{
	class Application {
	protected:
		bool _terminate{ false };
		GAPIContext* _gapi_context;

	public:
		Application() = default;
		Application(const Application &application) = delete;
		Application &operator=(const Application &application) = delete;

		virtual ~Application() { delete _gapi_context; }

		GAPIContext* get_context() const { return _gapi_context; }

		virtual bool initialize(int *argc, char *argv[]) = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual int run() = 0;
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
