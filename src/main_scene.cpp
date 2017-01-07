#include "main_scene.h"
#include "internal/engine_context.h"
#include "mesh_utils.h"
#include "resource_manager.h"
#include "camera_component.h"
#include "../GlacierEngine/rendering/include/render_state_manager.h"

using namespace Glacier;

static Object* sub1_root{ nullptr };
static Object* sub1_oar_root{ nullptr };

void MainScene::initialize()
{
	Object* obj{ new Object{ "globe" } };

	Mesh* m{ MeshUtils::generate_uv_sphere(1.0f, 30) };
	ResourceManager::register_resource(m, L"sphere");

	m = MeshUtils::generate_cube(1.0f);
	ResourceManager::register_resource(m, L"cube");

	Material mat;
	mat.diffuse = Vec4f{ 0.0470588235294118f, 0.3019607843137255f, 0.4117647058823529f, 0.5f };
	mat.specular = Vec4f{ 0.5f, 0.5f, 0.5f, 128.0f };

	RenderingComponent* rc{ new RenderingComponent{ obj } };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat);
	obj->set_position(Vec3f{ 0.0f, 0.0, 0.0f });
	obj->set_scale(Vec3f{ 5.0f, 5.0f, 5.0f });

	m_objects.push_back(obj);

	// Submarine 1 creation -------------------------------------------------------------------------------------------
	Object* sub1_controller{ new Object{ "sub1_controller" } };
	sub1_controller->set_euler_angles(Vec3f{ 0, -125.0f, 0 });
	sub1_root = sub1_controller;
	m_objects.push_back(sub1_controller);

	Object* body{ new Object{ "sub1_body" } };
	Material mat2;
	mat2.diffuse = Vec4f{ 0.0f, 1.0f, 1.0f, 1.0f };
	mat2.specular = Vec4f{ 1.0f, 1.0f, 1.0f, 60.0f };

	rc = new RenderingComponent{ body };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat2);
	body->set_scale(Vec3f{ 0.2f, 0.2f, 0.5f });
	body->set_parent(sub1_controller);
	m_objects.push_back(body);

	Object* top = new Object{ "sub1_top" };
	rc = new RenderingComponent{ top };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	top->set_position(Vec3f{ 0.0, 1.0, 0.0 });
	top->set_scale(Vec3f{ 1.0f, 1.0f, 0.6f });
	top->set_parent(body);
	m_objects.push_back(top);

	Object* side_fin1{ new Object{ "sub1_side_fin1" } };
	rc = new RenderingComponent{ side_fin1 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	side_fin1->set_position(Vec3f{ 0.0f, 0.0f, -0.7f });
	side_fin1->set_scale(Vec3f{ 1.5f, 0.2f, 0.4f });
	side_fin1->set_parent(body);
	m_objects.push_back(side_fin1);

	Object* side_fin2{ new Object{ "sub1_side_fin2" } };
	rc = new RenderingComponent{ side_fin2 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	side_fin2->set_position(Vec3f{ 0.0f, 0.0f, -0.7f });
	side_fin2->set_scale(Vec3f{ 1.5f, 0.2f, 0.4f });
	side_fin2->set_euler_angles(Vec3f{ 0.0f, 0.0f, 90.0f });
	side_fin2->set_parent(body);
	m_objects.push_back(side_fin2);

	Object* sub1_oar_controller{ new Object{"sub1_oar_controller"} };
	sub1_oar_controller->set_parent(body);
	sub1_oar_root = sub1_oar_controller;
	m_objects.push_back(sub1_oar_controller);

	Object *oar1{ new Object{"sub1_oar1"} };
	rc = new RenderingComponent{ oar1 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	oar1->set_position(Vec3f{ 1.5f, 0.0f, -0.37f });
	oar1->set_scale(Vec3f{ 1.5f, 0.2f, 0.1f });
	oar1->set_parent(sub1_oar_controller);

	m_objects.push_back(oar1);

	Object *oar2{ new Object{ "sub1_oar2" } };
	rc = new RenderingComponent{ oar2 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	oar2->set_position(Vec3f{ 1.5f, 0.0f, 0.37f });
	oar2->set_scale(Vec3f{ 1.5f, 0.2f, 0.1f });
	oar2->set_parent(sub1_oar_controller);

	m_objects.push_back(oar2);

	Object *oar3{ new Object{ "sub1_oar3" } };
	rc = new RenderingComponent{ oar3 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	oar3->set_position(Vec3f{ -1.5f, 0.0f, -0.37f });
	oar3->set_scale(Vec3f{ 1.5f, 0.2f, 0.1f });
	oar3->set_parent(sub1_oar_controller);

	m_objects.push_back(oar3);

	Object *oar4{ new Object{ "sub1_oar4" } };
	rc = new RenderingComponent{ oar4 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	oar4->set_position(Vec3f{ -1.5f, 0.0f, 0.37f });
	oar4->set_scale(Vec3f{ 1.5f, 0.2f, 0.1f });
	oar4->set_parent(sub1_oar_controller);

	m_objects.push_back(oar4);

	// -----------------------------------------------------------------------------------------------------------------

	Object* cam{ new Object{ "camera1" } };
	CameraComponent* cc{ new CameraComponent{ cam, MathUtils::to_radians(60.0f), 1024, 768, 0.1f, 1000.0f } };

	cam->set_position(Vec3f(0.0f, 0.0f, -8.0f));

	m_objects.push_back(cam);

	Object* cam2{ new Object{ "camera2" } };
	CameraComponent* cc2{ new CameraComponent{ cam2, MathUtils::to_radians(60.0f), 1024, 768, 0.1f, 1000.0f } };

	cam2->set_position(Vec3f(0.0f, 0.0f, -1.0f));

	m_objects.push_back(cam2);

	Object* light1{ new Object{ "light1" } };

	LightComponent* lc1{ new LightComponent{ light1 } };

	LightDesc light_desc;
	light_desc.ambient_intensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	light_desc.diffuse_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc.specular_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
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
	light_desc2.specular_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
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
	sub1_oar_root->set_euler_angles(Vec3f{ 0, cos(time / 300.0f) * 3.0f, sin(time / 300.0f) * 2.0f });
	//sub1_root->set_position(Vec3f{ cos(MathUtils::to_radians(time / 100.0f)) * 4.0f, 0.0f, sin(MathUtils::to_radians(time / 100.0f)) * 4.0f });

	Scene::update(delta_time, time);

	CameraSystem* camera_system{ EngineContext::get_camera_system() };

	camera_system->process(m_objects, delta_time);

	LightSystem* light_system{ EngineContext::get_light_system() };

	light_system->process(m_objects, delta_time);
}

void MainScene::draw() const noexcept
{
	RenderSystem* render_system{ EngineContext::get_render_system() };

	RenderStateManager::set(RenderStateType::BLEND_ALPHA);
	render_system->process(m_objects, 0);
	RenderStateManager::set(RenderStateType::BLEND_DISSABLED);
}
