#include "rtg_application.h"
#include <Windows.h>
#include <iostream>
#include "D3D11_context.h"
#define GLACIERENGINE_STATIC

using namespace Glacier;

RtgApplication::~RtgApplication()
{
	delete win;
	delete win2;
}

bool RtgApplication::initialize(int* argc, char* argv[])
{
	GAPIContext* gapi_context{ new D3D11Context() };
	gapi_context->create();
	GAPIContextLocator::provide(gapi_context);
	win = new Glacier::D3D11Window("D3D test", Vec2i{ 256, 256 }, Vec2i{ 250, 250 }, 0, true, false, true, true, true, 4);
	win2 = new Glacier::D3D11Window("D3D Test 2", Vec2i{ 500, 500 }, Vec2i{ 300, 300 }, 1, false, false, true, true, true, 4);

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
