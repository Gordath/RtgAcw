#include "main_scene.h"
#include "internal/engine_context.h"
#include "mesh_utils.h"
#include "resource_manager.h"

using namespace Glacier;

void MainScene::initialize()
{
	Object* obj{ new Object{"test_object"} };
	
	Mesh* m{ MeshUtils::generate_cube() };
	ResourceManager::register_resource(m, L"cube");

	RenderingComponent* rc{ new RenderingComponent{ obj } };
	rc->set_mesh(m);

	m_objects.push_back(obj);
}

void MainScene::on_key_down(int key, int x, int y) noexcept
{
}

void MainScene::on_key_up(int key, int x, int y) noexcept
{
}

void MainScene::draw() const noexcept
{
	RenderSystem* render_system{ EngineContext::get_render_system() };

	render_system->process(m_objects, 0);
}
