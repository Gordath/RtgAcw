#include "application.h"
#include <iostream>

namespace Glacier
{
	EngineContext Application::_engine_context;


	bool Application::initialize(int* argc, char* argv[])
	{
		if (!_engine_context.initialize()) {
			std::cerr << "Application initialization failed!" << std::endl;
			return false;
		}

		return true;
	}
}
