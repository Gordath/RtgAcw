#include "rtg_application.h"
#include <Windows.h>
#include <iostream>
#define GLACIERENGINE_STATIC

bool RtgApplication::initialize(int* argc, char* argv[])
{
	win = new Glacier::D3D11Window("D3D test", Vec2i{ 256, 256 }, Vec2i{ 250, 250 }, 0, true, false, true, true, true, 4);
	win = new Glacier::D3D11Window("D3D Test 2", Vec2i{ 500, 500 }, Vec2i{ 300, 300 }, 1, false, false, true, true, true, 4);
	return true;
}

void RtgApplication::update()
{
	
}

void RtgApplication::draw()
{
	
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
			
		}

	}

	return 0;
}
