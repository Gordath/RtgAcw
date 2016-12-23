#include "rtg_application.h"
#include <Windows.h>

using namespace Glacier;


// Private methods -----------------------------------------------------------------------------------------------------------------------------
void RtgApplication::reshape(int x, int y)
{
	//TODO: Call Renderer's reshape.
}

void RtgApplication::key_down(unsigned char key, int x, int y)
{
	//TODO: Call SceneManager's key down.
}

void RtgApplication::key_up(unsigned char key, int x, int y)
{
	//TODO: Call SceneManager's key up.
}

void RtgApplication::mouse_click(int button, bool state, int x, int y)
{
	//TODO: Call SceneManager's mouse click.
}

void RtgApplication::mouse_motion(int x, int y)
{
	//TODO: Call SceneManager's mouse motion.
}

void RtgApplication::passive_mouse_motion(int x, int y)
{
	//TODO: Call SceneManager's passive mouse motion.
}
// ---------------------------------------------------------------------------------------------------------------------------------------------


RtgApplication::~RtgApplication()
{
}

bool RtgApplication::initialize(int* argc, char* argv[])
{
	if (!Application::initialize(argc, argv)) {
		return false;
	}

	WindowFunctionCallbacks callbacks;
	callbacks.passive_motion_func = passive_mouse_motion;
	callbacks.motion_func = mouse_motion;
	callbacks.reshape_func = reshape;
	callbacks.keyboard_func = key_down;
	callbacks.keyboard_up_func = key_up;

	WindowingService::create(L"D3D test",
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
	//TODO: The scene manager will call update here.
}

void RtgApplication::draw() const noexcept
{
	//TODO: The scene manager will call draw here.

	WindowingService::swap_buffers();
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
			draw();
		}

	}

	return 0;
}
