#include "deep_sea_fish.h"
#include "../GlacierEngine/rendering/include/material.h"
#include "resource_manager.h"
#include "../GlacierEngine/rendering/include/d3d/D3D11_texture.h"
#include "../GlacierEngine/rendering/include/mesh.h"
#include "rendering_component.h"
#include "emitter_conponent.h"

using namespace Glacier;

void DeepSeaFish::update(float dt, long time) noexcept
{
	// animate fins and tail
	m_right_fin_controller->set_euler_angles(Vec3f{ 0.0f, MathUtils::to_degrees(sin(time / 200.0f) * 0.05f), 0.0f });
	m_left_fin_controller->set_euler_angles(Vec3f{ 0.0f, -MathUtils::to_degrees(sin(time / 200.0f) * 0.05f), 0.0f });
	m_tail_controller->set_euler_angles(Vec3f{ 0.0f, MathUtils::to_degrees(sin(time / 200.0f) * 0.15f), 0.0f });
	Object::update(dt, time);
}

void DeepSeaFish::setup() noexcept
{
	//Self init
	Object::setup();
	m_scene->add_object(this);
	//----

	Object* body{ new Object{ "fish_body" } };
	Glacier::Material mat2;
	mat2.diffuse = Vec4f{ MathUtils::random_range(0.2f, 1.0f), MathUtils::random_range(0.2f, 1.0f), MathUtils::random_range(0.2f, 1.0f), 1.0f };
	mat2.specular = Vec4f{ 0.5f, 0.5f, 0.5f, 20.0f };
	mat2.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"fish_diff.png");
	mat2.textures[TEX_SPECULAR] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"fish_spec.png");
	mat2.textures[TEX_NORMAL] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"fish_norm.png");
	mat2.textures[TEX_DIFFUSE]->set_texture_type(TEX_DIFFUSE);
	mat2.textures[TEX_SPECULAR]->set_texture_type(TEX_SPECULAR);
	mat2.textures[TEX_NORMAL]->set_texture_type(TEX_NORMAL);
	mat2.texture_matrix = MathUtils::scale(Mat4f{}, Vec3f{ 4.0f, 4.0f, 4.0f });

	RenderingComponent* rc{ new RenderingComponent{ body } };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat2);
	body->set_scale(Vec3f{ 0.08f, 0.1f, 0.4f });
	body->set_parent(this);
	body->setup();
	m_scene->add_object(body);

	m_right_fin_controller = new Object{ "right_fin_controller" };
	m_right_fin_controller->set_parent(body);
	m_scene->add_object(m_right_fin_controller);

	Object* right_fin{ new Object{"right_fin"} };
	rc = new RenderingComponent{ right_fin };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	mat2.texture_matrix = MathUtils::scale(Mat4f{}, Vec3f{ 2.0f, 2.0f, 2.0f });
	rc->set_material(mat2);
	right_fin->set_scale(Vec3f{ 1.2, 0.3, 0.02 });
	right_fin->set_position(Vec3f{ 0.7f, 0.0f, 0.5f });
	right_fin->set_parent(m_right_fin_controller);
	right_fin->setup();
	m_scene->add_object(right_fin);


	m_left_fin_controller = new Object{ "left_fin_controller" };
	m_left_fin_controller->set_parent(body);
	m_scene->add_object(m_left_fin_controller);

	Object* right_fin2{ new Object{ "right_fin2" } };
	rc = new RenderingComponent{ right_fin2 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat2);
	right_fin2->set_scale(Vec3f{ 1.2, 0.3, 0.02 });
	right_fin2->set_position(Vec3f{ -0.7f, 0.0f, 0.5f });
	right_fin2->set_parent(m_left_fin_controller);
	right_fin2->setup();
	m_scene->add_object(right_fin2);

	m_tail_controller = new Object{ "tail_controller" };
	m_tail_controller->set_parent(body);
	m_scene->add_object(m_tail_controller);

	Object* tail{ new Object{"tail"} };
	rc = new RenderingComponent{ tail };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat2);
	tail->set_scale(Vec3f{ 0.1, 0.65, 0.4 });
	tail->set_position(Vec3f{ 0.0f, 0.0f, -0.8f });
	tail->set_parent(m_tail_controller);
	tail->setup();
	m_scene->add_object(tail);

	Object* top_fin{ new Object{ "top_fin" } };
	rc = new RenderingComponent{ top_fin };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat2);
	top_fin->set_scale(Vec3f{ 0.1, 0.7, 0.6 });
	top_fin->set_position(Vec3f{ 0.0f, 0.7f, 0.0f });
	top_fin->set_parent(body);
	top_fin->setup();
	m_scene->add_object(top_fin);

	Object* left_eye{ new Object{ "left_eye" } };
	rc = new RenderingComponent{ left_eye };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	mat2.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyDiff.jpg");
	mat2.textures[TEX_SPECULAR] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummySpec.jpg");
	mat2.textures[TEX_NORMAL] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyNorm.png");
	rc->set_material(mat2);
	left_eye->set_scale(Vec3f{ 0.2, 0.2, 0.05 });
	left_eye->set_position(Vec3f{ -0.55f, 0.0f, 0.8f });
	left_eye->set_parent(body);
	left_eye->setup();
	m_scene->add_object(left_eye);

	Object* right_eye{ new Object{ "right_eye" } };
	rc = new RenderingComponent{ right_eye };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	mat2.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyDiff.jpg");
	mat2.textures[TEX_SPECULAR] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummySpec.jpg");
	mat2.textures[TEX_NORMAL] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyNorm.png");
	rc->set_material(mat2);
	right_eye->set_scale(Vec3f{ 0.2, 0.2, 0.05 });
	right_eye->set_position(Vec3f{ 0.55f, 0.0f, 0.8f });
	right_eye->set_parent(body);
	right_eye->setup();
	m_scene->add_object(right_eye);

	Object* emitter{ new Object{ "fish_emmiter1" } };
	EmitterComponent* ec{ new EmitterComponent{ emitter } };
	ec->set_lifespan(1.0f);
	ec->set_max_particles(1000);
	ec->set_spawn_rate(1.0);
	ec->set_active(true);
	ec->set_particle_size(0.05f);
	ec->set_spawn_radius(0.01f);
	ec->set_velocity(Vec3f{ 0.0f, 1.0f, 0.0f });
	ec->set_velocity_range(0.3f);
	ec->set_external_force(Vec3f{ 0.0f, 0.3f, 0.0f });
	ec->set_mesh(ResourceManager::get<Mesh>(L"plane"));
	ec->set_start_color(Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f });
	ec->set_end_color(Vec4f{ 0.1f, 0.3f, 1.0f, 0.3f });
	Material p_mat;
	p_mat.blend_state = RenderStateType::BS_BLEND_ADDITIVE;
	p_mat.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"bubble10.png");
	p_mat.textures[TEX_DIFFUSE]->set_texture_type(TEX_DIFFUSE);
	ec->set_material(p_mat);
	emitter->set_position(Vec3f{ 0.0f, 0.0f, 1.0f });
	emitter->set_parent(body);
	emitter->setup();
	m_scene->add_object(emitter);
}

void DeepSeaFish::teardown() noexcept
{
}
