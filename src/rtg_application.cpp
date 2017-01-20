#include "rtg_application.h"
#include <Windows.h>
#include "shader_program_manager.h"
#include "scene.h"
#include "main_scene.h"
#include "timer.h"
#include <iostream>
#include "scene_manager.h"
#include "../GlacierEngine/windowing/include/windowing_service.h"

using namespace Glacier;

static Scene* scene{ nullptr };

// Private methods -----------------------------------------------------------------------------------------------------------------------------
void RtgApplication::reshape(int x, int y)
{
	//TODO: Call Renderer's reshape.
}

void RtgApplication::key_down(unsigned char key, int x, int y)
{
	SceneManager::on_key_down(key, x, y);
}

void RtgApplication::key_up(unsigned char key, int x, int y)
{
	SceneManager::on_key_up(key, x, y);
}

void RtgApplication::mouse_click(int button, bool state, int x, int y)
{
	SceneManager::on_mouse_click(button, state, x, y);
}

void RtgApplication::mouse_motion(int x, int y)
{
	SceneManager::on_mouse_motion(x, y);
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
	callbacks.mouse_func = mouse_click;
	callbacks.reshape_func = reshape;
	callbacks.keyboard_func = key_down;
	callbacks.keyboard_up_func = key_up;
	callbacks.mouse_func = mouse_click;


	WindowingService::create(L"RtgAcw",
	                          Vec2i{ 1280, 800 },
	                          Vec2i{ 250, 250 },
	                          true,
	                          false,
	                          true,
	                          true,
	                          true,
	                          4,
	                          callbacks);

	if (!ShaderProgramManager::create("color_pass_sdrprog", IL_POSITION | IL_NORMAL | IL_TANGENT | IL_TEXCOORD | IL_COLOR, L"color_pass.vs.hlsl", L"color_pass.ps.hlsl")) {
		return false;
	}

	if (!ShaderProgramManager::create("render_texture_sdrprog", IL_POSITION | IL_TEXCOORD, L"render_texture.vs.hlsl", L"render_texture.ps.hlsl")) {
		return false;
	}

	if (!ShaderProgramManager::create("depth_pass_sdrprog", IL_POSITION, L"depth_pass.vs.hlsl", L"depth_pass.ps.hlsl")) {
		return false;
	}

	if (!ShaderProgramManager::create("particles_sdrprog", IL_POSITION | IL_TEXCOORD, L"particles.vs.hlsl", L"particles.ps.hlsl")) {
		return false;
	}

	if (!ShaderProgramManager::create("skybox_sdrprog", IL_POSITION | IL_NORMAL, L"skybox.vs.hlsl", L"skybox.ps.hlsl")) {
		return false;
	}

	scene = new MainScene;
	SceneManager::push_scene(scene);

	return true;
}

void RtgApplication::update() noexcept
{
	SceneManager::update(m_timer.get_delta(), m_timer.get_msec());
}

void RtgApplication::draw() const noexcept
{
	SceneManager::draw();

	WindowingService::swap_buffers();
}

int RtgApplication::run() noexcept
{
	MSG msg;

	while (!m_terminate) {

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
