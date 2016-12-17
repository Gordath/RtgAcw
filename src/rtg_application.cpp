#include "rtg_application.h"
#include <Windows.h>
#include <iostream>

using namespace Glacier;

RtgApplication::~RtgApplication()
{

}

bool RtgApplication::initialize(int* argc, char* argv[])
{
	if(!_engine_context.initialize()) {
		std::cerr << "RtgApplication initialization failed!" << std::endl;
		return false;
	}

	WindowingService* windowing_service{ get_windowing_service() };
	windowing_service->create("D3D test", Vec2i{ 256, 256 }, Vec2i{ 250, 250 }, true, false, true, true, true, 4);

	return true;
}

void RtgApplication::update()
{
	std::cout << "Updating" << std::endl;
}

void RtgApplication::draw()
{
	std::cout << "Drawing" << std::endl;
}

int RtgApplication::run()
{
	MSG msg;

	while (!_terminate) {
		
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (WM_QUIT == msg.message) {
				break;
			}
		}
		else {
			update();
			draw();
		}

	}

	return 0;
}
