#include "main_scene.h"
#include "internal/engine_context.h"


void MainScene::initialize()
{
	
}

void MainScene::on_key_down(int key, int x, int y) noexcept
{
}

void MainScene::on_key_up(int key, int x, int y) noexcept
{
}

void MainScene::draw() const noexcept
{
	auto render_system{ Glacier::EngineContext::get_render_system() };

	render_system->process(m_objects, 0);
}
