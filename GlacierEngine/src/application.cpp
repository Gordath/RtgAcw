#include "application.h"
#include <iostream>

namespace Glacier
{
	EngineContext Application::m_engine_context;


	bool Application::initialize(int* argc, char* argv[])
	{
		if (!m_engine_context.initialize()) {
			std::cerr << "Application initialization failed!" << std::endl;
			return false;
		}

		m_timer.start();

		return true;
	}
}
