#ifndef GLACIER_ENGINE_APPLICATION_H_
#define GLACIER_ENGINE_APPLICATION_H_
#include "dll_export.h"
#include "types.h"
#include <string>
#include "windowing_service.h"

namespace Glacier
{
	class Application {
	protected:
		bool _terminate;

	public:
		Application() = default;
		Application(const Application &application) = delete;
		Application &operator=(const Application &application) = delete;

		virtual ~Application() = default;

		virtual bool initialize(int *argc, char *argv[]) = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual int run() = 0;
	};
}

#endif //GLACIER_ENGINE_APPLICATION_H_
