#include "rtg_application.h"
#include <Windows.h>

bool RtgApplication::initialize(int* argc, char* argv[])
{
	win = new Glacier::D3D11Window("D3D test", Vec2i{ 2048, 2048 }, Vec2i{ 250, 250 }, 0, true, false, true, true, true, 4);
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
		
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			switch (msg.message) {
			case WM_APP:
				if (WM_QUIT == msg.lParam) {
					PostQuitMessage(msg.lParam);
				}
				break;
			case WM_QUIT:
				_terminate = true;
			default:
				break;
			}
		}

	}

	return 0;
}
