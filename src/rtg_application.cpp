#include "rtg_application.h"
#include <Windows.h>
#include <iostream>

using namespace Glacier;

RtgApplication::~RtgApplication()
{
}

bool RtgApplication::initialize(int* argc, char* argv[])
{
	if (!_engine_context.initialize()) {
		std::cerr << "RtgApplication initialization failed!" << std::endl;
		return false;
	}

	WindowingService* windowing_service{ get_windowing_service() };

	WindowFunctionCallbacks callbacks;
	callbacks.passive_motion_func = [](int x, int y) { std::cout << "Passive motion:" << x << " " << y << std::endl; };
	callbacks.motion_func = [](int x, int y) { std::cout << "Motion:" << x << " " << y << std::endl; };
	callbacks.reshape_func = [](int x, int y) { std::cout << "Reshape:" << x << " " << y << std::endl; };
	callbacks.keyboard_func = [](unsigned char key, int x, int y) { std::cout << "Keyboard:" << key << " " << x << " " << y << std::endl; };
	callbacks.keyboard_up_func = [](unsigned char key, int x, int y) { std::cout << "KeyboardUp:" << key << " " << x << " " << y << std::endl; };

	windowing_service->create(L"D3D test",
	                          Vec2i{ 256, 256 },
	                          Vec2i{ 250, 250 },
	                          true,
	                          false,
	                          true,
	                          true,
	                          true,
	                          4,
	                          callbacks);

	return true;
}

void RtgApplication::update() noexcept
{
	//std::cout << "Updating" << std::endl;
}

void RtgApplication::draw() noexcept
{
	//std::cout << "Drawing" << std::endl;
}

int RtgApplication::run() noexcept
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
		}

	}

	return 0;
}
