#include "main_scene.h"
#include "internal/engine_context.h"
#include "mesh_utils.h"
#include "resource_manager.h"
#include "camera_component.h"

using namespace Glacier;

void MainScene::initialize()
{
	Object* obj{ new Object{ "test_object" } };

	Mesh* m{ MeshUtils::generate_uv_sphere(1.0f, 30) };
	ResourceManager::register_resource(m, L"cube");

	RenderingComponent* rc{ new RenderingComponent{ obj } };
	rc->set_mesh(m);
	obj->set_position(Vec3f{ 0.0f, 0.0, 2.0f });
	

	m_objects.push_back(obj);

	Object* cam{ new Object{ "camera1" } };
	CameraComponent* cc{ new CameraComponent{ cam, MathUtils::to_radians(60.0f), 1024, 768, 0.1f, 1000.0f } };

	cam->set_position(Vec3f(0.0f, 0.0f, 0.0f));

	m_objects.push_back(cam);

	Object* cam2{ new Object{ "camera2" } };
	CameraComponent* cc2{ new CameraComponent{ cam2, MathUtils::to_radians(60.0f), 1024, 768, 0.1f, 1000.0f } };

	cam2->set_position(Vec3f(0.0f, 0.0f, -2.0f));

	m_objects.push_back(cam2);

	Object* light1{ new Object{"light1"} };

	LightComponent* lc1{ new LightComponent{light1} };

	LightDesc light_desc;
	light_desc.ambient_intensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	light_desc.diffuse_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc.specular_intensity = Vec4f{ 0.0f, 1.0f, 0.0f, 1.0f };
	light_desc.flags = Vec4ui{ 1, 1, 0, 0 };
	light_desc.attenuation = Vec3f{ 1.0f, 0.0f, 0.0f };

	lc1->set_light_description(light_desc);

	light1->set_position(Vec3f{ -1.0f, 0.0, 0.0f });

	m_objects.push_back(light1);

	Object* light2{ new Object{ "light2" } };

	LightComponent* lc2{ new LightComponent{ light2 } };

	LightDesc light_desc2;
	light_desc2.ambient_intensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	light_desc2.diffuse_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc2.specular_intensity = Vec4f{ 0.0f, 0.0f, 1.0f, 1.0f };
	light_desc2.flags = Vec4ui{ 1, 1, 0, 0 };
	light_desc2.attenuation = Vec3f{ 1.0f, 0.0f, 0.0f };

	lc2->set_light_description(light_desc2);

	light2->set_position(Vec3f{ 0.0f, 0.0, -1.0f });

	m_objects.push_back(light2);
}

void MainScene::on_key_down(unsigned char key, int x, int y) noexcept
{
}

void MainScene::on_key_up(unsigned char key, int x, int y) noexcept
{
	switch (key) {
	case 'A':
		EngineContext::get_camera_system()->set_active_camera("camera1");
		break;
	case 'S':
		EngineContext::get_camera_system()->set_active_camera("camera2");
		break;
	default:
		break;
	}
}

void MainScene::on_mouse_motion(int x, int y) noexcept
{
	EngineContext::get_camera_system()->get_active_camera()->set_euler_angles(Vec3f{ y, x, 0 });
}

void MainScene::on_mouse_click(int button, bool state, int x, int y)
{

}

void MainScene::update(float delta_time, long time) noexcept
{
	Scene::update(delta_time, time);

	CameraSystem* camera_system{ EngineContext::get_camera_system() };

	camera_system->process(m_objects, delta_time);

	LightSystem* light_system{ EngineContext::get_light_system() };

	light_system->process(m_objects, delta_time);
}

void MainScene::draw() const noexcept
{
	RenderSystem* render_system{ EngineContext::get_render_system() };

	render_system->process(m_objects, 0);
}
