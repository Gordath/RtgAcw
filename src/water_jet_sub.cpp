#include "water_jet_sub.h"
#include "../GlacierEngine/rendering/include/material.h"
#include "resource_manager.h"
#include "../GlacierEngine/rendering/include/d3d/D3D11_texture.h"
#include "rendering_component.h"
#include "emitter_conponent.h"

using namespace Glacier;

void WaterJetSubmarine::update(float dt, long time) noexcept
{
	Object::update(dt, time);
}

void WaterJetSubmarine::setup() noexcept
{
	//Self init
	Object::setup();
	m_scene->add_object(this);
	//----

	Object* body{ new Object{ "sub2_body" } };
	Material mat2;
	mat2.diffuse = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	mat2.specular = Vec4f{ 1.0f, 1.0f, 1.0f, 20.0f };
	mat2.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"jet_sub_diff.png");
	mat2.textures[TEX_SPECULAR] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"jet_sub_spec.png");
	mat2.textures[TEX_NORMAL] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"jet_sub_norm.png");
	mat2.textures[TEX_DIFFUSE]->set_texture_type(TEX_DIFFUSE);
	mat2.textures[TEX_SPECULAR]->set_texture_type(TEX_SPECULAR);
	mat2.textures[TEX_NORMAL]->set_texture_type(TEX_NORMAL);
	mat2.texture_matrix = MathUtils::scale(Mat4f{}, Vec3f{ 3.0f, 3.0f, 3.0f });

	RenderingComponent* rc{ new RenderingComponent{ body } };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat2);
	body->set_scale(Vec3f{ 0.2f, 0.2f, 0.7f });
	body->set_parent(this);
	body->setup();
	m_scene->add_object(body);

	Object* top = new Object{ "sub2_top" };
	rc = new RenderingComponent{ top };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat2);
	top->set_position(Vec3f{ 0.0, 0.7, 0.0 });
	top->set_scale(Vec3f{ 0.4f, 1.0f, 0.4f });
	top->set_parent(body);
	top->setup();
	m_scene->add_object(top);

	Object* side_fin1{ new Object{ "sub1_side_fin1" } };
	rc = new RenderingComponent{ side_fin1 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	side_fin1->set_position(Vec3f{ 0.0f, 0.0f, -0.7f });
	side_fin1->set_scale(Vec3f{ 1.5f, 0.2f, 0.4f });
	side_fin1->set_parent(body);
	side_fin1->setup();
	m_scene->add_object(side_fin1);

	Object* side_fin2{ new Object{ "sub1_side_fin2" } };
	rc = new RenderingComponent{ side_fin2 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	side_fin2->set_position(Vec3f{ 0.0f, 0.0f, -0.7f });
	side_fin2->set_scale(Vec3f{ 1.5f, 0.2f, 0.4f });
	side_fin2->set_euler_angles(Vec3f{ 0.0f, 0.0f, 90.0f });
	side_fin2->set_parent(body);
	side_fin2->setup();
	m_scene->add_object(side_fin2);

	Object* oar1{ new Object{ "sub1_oar1" } };
	rc = new RenderingComponent{ oar1 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat2);
	oar1->set_position(Vec3f{ 0.8f, 0.0f, -0.37f });
	oar1->set_scale(Vec3f{ 0.3f, 0.3f, 0.1f });
	oar1->set_parent(body);
	oar1->setup();

	m_scene->add_object(oar1);

	Object* oar2{ new Object{ "sub1_oar2" } };
	rc = new RenderingComponent{ oar2 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat2);
	oar2->set_position(Vec3f{ 0.8f, 0.0f, 0.37f });
	oar2->set_scale(Vec3f{ 0.3f, 0.3f, 0.1f });
	oar2->set_parent(body);
	oar2->setup();
	m_scene->add_object(oar2);

	Object* oar3{ new Object{ "sub1_oar3" } };
	rc = new RenderingComponent{ oar3 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat2);
	oar3->set_position(Vec3f{ -0.8f, 0.0f, -0.37f });
	oar3->set_scale(Vec3f{ 0.3f, 0.3f, 0.1f });
	oar3->set_parent(body);
	oar3->setup();
	m_scene->add_object(oar3);

	Object* oar4{ new Object{ "sub1_oar4" } };
	rc = new RenderingComponent{ oar4 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat2);
	oar4->set_position(Vec3f{ -0.8f, 0.0f, 0.37f });
	oar4->set_scale(Vec3f{ 0.3f, 0.3f, 0.1f });
	oar4->set_parent(body);
	oar4->setup();
	m_scene->add_object(oar4);

	Object* emitter{ new Object{ "emmiter1" } };
	EmitterComponent* ec{ new EmitterComponent{ emitter } };
	ec->set_lifespan(6.0);
	ec->set_max_particles(1000);
	ec->set_spawn_rate(20.0);
	ec->set_active(true);
	ec->set_particle_size(0.3f);
	ec->set_spawn_radius(0.2f);
	ec->set_velocity(Vec3f{ 0.0f, 0.0f, 0.0f });
	ec->set_velocity_range(0.1f);
	ec->set_external_force(Vec3f{ 0.0f, 0.1f, 0.0f });
	ec->set_mesh(ResourceManager::get<Mesh>(L"plane"));
	ec->set_start_color(Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f });
	ec->set_end_color(Vec4f{ 1.0f, 1.0f, 1.0f, 0.3f });
	Material p_mat;
	p_mat.blend_state = RenderStateType::BS_BLEND_ADDITIVE;
	p_mat.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"water.png");
	p_mat.textures[TEX_DIFFUSE]->set_texture_type(TEX_DIFFUSE);
	ec->set_material(p_mat);
	emitter->set_position(Vec3f{ 0.0f, 0.0f, -2.0f });
	emitter->set_parent(body);
	emitter->setup();
	m_scene->add_object(emitter);
}
