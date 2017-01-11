#include "main_scene.h"
#include "internal/engine_context.h"
#include "mesh_utils.h"
#include "resource_manager.h"
#include "camera_component.h"
#include "../GlacierEngine/rendering/include/render_state_manager.h"
#include "../GlacierEngine/windowing/include/windowing_service.h"
#include "shader_program_manager.h"

using namespace Glacier;


static Object* sub1_root{ nullptr };
static Object* sub1_oar_root{ nullptr };

static float cam_theta, cam_phi, cam_dist = 16;

static bool wireframe{ false };

struct ColorPassUniformBuffer {
	Mat4f MVP;
	Mat4f MV;
	Mat4f M;
	Mat4f V;
	Mat4f P;
	Mat4f ITMV;
	Vec4f diffuse;
	Vec4f specular;
};

struct DepthPassUniformBuffer {
	Mat4f MVP;
};

void MainScene::depth_pass() const noexcept
{
	D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };
	ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

	std::vector<RenderingComponent*> rendering_components;
	for (auto object : m_objects) {
		RenderingComponent* rc{ static_cast<RenderingComponent*>(object->get_component("co_rendering")) };

		if (rc) {
			rendering_components.push_back(rc);
		}
	}

	auto lights{ EngineContext::get_light_system()->get_active_light_descriptions() };

	float clear_color[4]{ 0.0f, 0.0f, 0.0f, 1.0f };

	RenderStateManager::set(RenderStateType::RS_CULL_FRONT);

	for (int i = 0; i < lights.size() ; ++i) {
		m_depth_pass_rts[i].bind(RenderTargetBindType::DEPTH);
		m_depth_pass_rts[i].clear(clear_color);

		if (lights[i].flags.y == true) {
			ShaderProgramManager::get("depth_pass_sdrprog")->bind();

			for (auto rendering_component : rendering_components) {

				if (rendering_component->get_mesh() && rendering_component->should_draw()) {

					Mat4f model{ rendering_component->get_xform() };
					Mat4f light_view{ lights[i].light_view_matrix };
					Mat4f light_projection{ lights[i].light_projection_matrix };

					Mat4f MVP{ light_projection * light_view * model };

					DepthPassUniformBuffer uniforms;
					uniforms.MVP = MathUtils::transpose(MVP);

					D3D11_MAPPED_SUBRESOURCE ms;
					device_context->Map(m_depth_pass_uniform_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
					memcpy(ms.pData, &uniforms, sizeof(DepthPassUniformBuffer));
					device_context->Unmap(m_depth_pass_uniform_buffer.Get(), 0);

					device_context->VSSetConstantBuffers(0, 1, m_depth_pass_uniform_buffer.GetAddressOf());

					D3D11_VIEWPORT viewport;
					viewport.TopLeftX = 0.0f;
					viewport.TopLeftY = 0.0f;
					viewport.Width = static_cast<float>(m_depth_pass_rts->get_size().x);
					viewport.Height = static_cast<float>(m_depth_pass_rts->get_size().y);
					viewport.MinDepth = 0.0f;
					viewport.MaxDepth = 1.0f;

					device_context->RSSetViewports(1, &viewport);

					Mesh* mesh{ rendering_component->get_mesh() };

					mesh->get_vbo()->bind();

					if (mesh->get_index_count()) {
						mesh->get_ibo()->bind();
						mesh->get_ibo()->draw();
					}
					else {
						mesh->get_vbo()->draw();
					}
				}
			}
		}
		
		m_depth_pass_rts[i].unbind();
	}

	RenderStateManager::set(RenderStateType::RS_CULL_BACK);
	
}

void MainScene::color_pass() const noexcept
{
	float clear_color[4]{ 0.0f, 0.0f, 0.0f, 1.0f };

	m_color_pass_rt.bind(RenderTargetBindType::COLOR_AND_DEPTH);
	m_color_pass_rt.clear(clear_color);

	ShaderProgramManager::get("color_pass_sdrprog")->bind();

	D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };
	ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

	device_context->PSSetShaderResources(0, 1, m_light_srv.GetAddressOf());
	device_context->PSSetSamplers(0, 1, m_sampler_linear_wrap.GetAddressOf());
	device_context->PSSetSamplers(1, 1, m_sampler_linear_clamp.GetAddressOf());

	std::vector<ID3D11ShaderResourceView*> depth_textures;
	for (int i = 0; i < 4; ++i) {
		depth_textures.push_back(m_depth_pass_rts[i].get_depth_attachment());
	}

	device_context->PSSetShaderResources(4, 4, depth_textures.data());

	std::vector<RenderingComponent*> rendering_components;
	for (auto object : m_objects) {
		RenderingComponent* rc{ static_cast<RenderingComponent*>(object->get_component("co_rendering")) };

		if (rc) {
			rendering_components.push_back(rc);
		}
	}

	std::sort(rendering_components.begin(),
	          rendering_components.end(),
	          [](auto a, auto b) {
		          return a->get_material().diffuse.w > b->get_material().diffuse.w;
	          }
	);

	RenderStateManager::set(RenderStateType::BS_BLEND_ALPHA);

	if (wireframe) {
		RenderStateManager::set(RenderStateType::RS_DRAW_WIRE);
	}

	Mat4f cam_matrix = Mat4f(1.0);
	cam_matrix = MathUtils::rotate(cam_matrix, MathUtils::to_radians(cam_theta), Vec3f(0, 1, 0));
	cam_matrix = MathUtils::rotate(cam_matrix, MathUtils::to_radians(cam_phi), Vec3f(1, 0, 0));
	cam_matrix = MathUtils::translate(cam_matrix, Vec3f(0, 0, -cam_dist));

	for (auto rendering_component : rendering_components) {

		if (rendering_component->get_mesh() && rendering_component->should_draw()) {
			CameraSystem* camera_system{ EngineContext::get_camera_system() };

			//Mat4f view{ camera_system->get_active_camera_cam_matrix() };
			Mat4f view = MathUtils::inverse(cam_matrix);

			Mat4f projection{ camera_system->get_active_camera_projection_matrix() };

			Mat4f model{ rendering_component->get_xform() };

			Mat4f MVP{ projection * view * model };
			Mat4f MV{ view * model };
			Mat4f ITMV{ MathUtils::transpose(MathUtils::inverse(MV)) };

			Material material{ rendering_component->get_material() };

			ColorPassUniformBuffer uniforms;
			uniforms.MVP = MathUtils::transpose(MVP);
			uniforms.MV = MathUtils::transpose(MV);
			uniforms.M = MathUtils::transpose(model);
			uniforms.V = MathUtils::transpose(view);
			uniforms.P = MathUtils::transpose(projection);
			uniforms.ITMV = MathUtils::transpose(ITMV);
			uniforms.diffuse = material.diffuse;
			uniforms.specular = material.specular;

			D3D11_MAPPED_SUBRESOURCE ms;

			device_context->Map(m_color_pass_uniform_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			memcpy(ms.pData, &uniforms, sizeof(ColorPassUniformBuffer));
			device_context->Unmap(m_color_pass_uniform_buffer.Get(), 0);

			device_context->VSSetConstantBuffers(0, 1, m_color_pass_uniform_buffer.GetAddressOf());
			device_context->PSSetConstantBuffers(0, 1, m_color_pass_uniform_buffer.GetAddressOf());

			D3D11_VIEWPORT viewport;
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;
			viewport.Width = static_cast<float>(WindowingService::get_window(0)->get_size().x);
			viewport.Height = static_cast<float>(WindowingService::get_window(0)->get_size().y);
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;

			device_context->RSSetViewports(1, &viewport);

			Mesh* mesh{ rendering_component->get_mesh() };

			mesh->get_vbo()->bind();

			if (mesh->get_index_count()) {
				mesh->get_ibo()->bind();
				mesh->get_ibo()->draw();
			}
			else {
				mesh->get_vbo()->draw();
			}
		}
	}

	RenderStateManager::set(RenderStateType::BS_BLEND_DISSABLED);

	m_color_pass_rt.unbind();
}

void MainScene::display_to_screen() const noexcept
{
	D3D11Context* ctx{ EngineContext::get_GAPI_context() };

	ComPtr<ID3D11DeviceContext> dev_con{ ctx->get_device_context() };

	dev_con->OMSetRenderTargets(1, ctx->get_address_of_render_target_view(), ctx->get_default_depth_stencil_view());

	float cl_col[4]{ 0.0, 0.0, 0.0, 0.0 };
	dev_con->ClearRenderTargetView(ctx->get_default_render_target_view(), cl_col);
	dev_con->ClearDepthStencilView(ctx->get_default_depth_stencil_view(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ShaderProgramManager::get("render_texture_sdrprog")->bind();

	ComPtr<ID3D11ShaderResourceView> srv{ m_color_pass_rt.get_color_attachment() };
	dev_con->PSSetShaderResources(0, 1, srv.GetAddressOf());
	dev_con->PSSetSamplers(0, 1, m_sampler_linear_wrap.GetAddressOf());
	
	RenderStateManager::set(RenderStateType::RS_DRAW_SOLID);
	dev_con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	dev_con->Draw(4, 0);

	ID3D11ShaderResourceView* null_srv{ nullptr };
	dev_con->PSSetShaderResources(0, 1, &null_srv);
}


void MainScene::initialize()
{
	ColorPassUniformBuffer cp_uniforms;

	D3D11_BUFFER_DESC cb_desc;
	ZeroMemory(&cb_desc, sizeof(cb_desc));
	cb_desc.ByteWidth = sizeof(cp_uniforms);
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;

	// Create the buffer.
	D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };
	ComPtr<ID3D11Device> device{ GAPI_context->get_device() };

	HRESULT res{ device->CreateBuffer(&cb_desc, nullptr, m_color_pass_uniform_buffer.ReleaseAndGetAddressOf()) };

	if (FAILED(res)) {
		std::cerr << "Renderer initialization failed: Uniform buffer creation failed." << std::endl;
	}

	DepthPassUniformBuffer dp_uniforms;
	ZeroMemory(&cb_desc, sizeof(cb_desc));
	cb_desc.ByteWidth = sizeof(dp_uniforms);
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;

	res = device->CreateBuffer(&cb_desc, nullptr, m_depth_pass_uniform_buffer.ReleaseAndGetAddressOf());

	if (FAILED(res)) {
		std::cerr << "Renderer initialization failed: Uniform buffer creation failed." << std::endl;
	}

	// LIGHTS STRUCTURED 
	D3D11_BUFFER_DESC sb_desc;
	ZeroMemory(&sb_desc, sizeof(sb_desc));
	sb_desc.ByteWidth = 4 * sizeof(LightDesc);
	sb_desc.Usage = D3D11_USAGE_DYNAMIC;
	sb_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sb_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	sb_desc.StructureByteStride = sizeof(LightDesc);


	res = device->CreateBuffer(&sb_desc, nullptr, m_light_structured_buffer.ReleaseAndGetAddressOf());

	if (FAILED(res)) {
		std::cerr << "Active light structured buffer creation failed." << std::endl;
	}

	res = device->CreateShaderResourceView(m_light_structured_buffer.Get(), nullptr, m_light_srv.ReleaseAndGetAddressOf());

	if (FAILED(res)) {
		std::cerr << "Light structured buffer shader resource view creation failed." << std::endl;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	res = device->CreateSamplerState(&samplerDesc, m_sampler_linear_wrap.ReleaseAndGetAddressOf());

	if (FAILED(res)) {
		std::cerr << "Linear Texture Wrap sampler creation failed!" << std::endl;
	}

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	
	res = device->CreateSamplerState(&samplerDesc, m_sampler_linear_clamp.ReleaseAndGetAddressOf());

	if (FAILED(res)) {
		std::cerr << "Linear Texture Wrap sampler creation failed!" << std::endl;
	}

	Object* obj{ new Object{ "globe" } };

	Mesh* m{ MeshUtils::generate_uv_sphere(1.0f, 60, 60) };
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

	//m_objects.push_back(obj);

	obj = new Object{ "ground" };
	rc = new RenderingComponent{ obj };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	mat.diffuse = Vec4f{ 1.00f, 1.00f, 1.00f, 1.0f };
	mat.specular = Vec4f{ 0.0f };
	rc->set_material(mat);
	obj->set_position(Vec3f{ 0.0f, -5.0, 0.0f });
	obj->set_scale(Vec3f{ 500.0f, 0.1f, 500.0f });

	m_objects.push_back(obj);

	// Submarine 1 creation -------------------------------------------------------------------------------------------
	Object* sub1_controller{ new Object{ "sub1_controller" } };
	sub1_controller->set_euler_angles(Vec3f{ 0, 45.0f, 0 });
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

	Object* sub1_oar_controller{ new Object{ "sub1_oar_controller" } };
	sub1_oar_controller->set_parent(body);
	sub1_oar_root = sub1_oar_controller;
	m_objects.push_back(sub1_oar_controller);

	Object* oar1{ new Object{ "sub1_oar1" } };
	rc = new RenderingComponent{ oar1 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	oar1->set_position(Vec3f{ 1.5f, 0.0f, -0.37f });
	oar1->set_scale(Vec3f{ 1.5f, 0.2f, 0.1f });
	oar1->set_parent(sub1_oar_controller);

	m_objects.push_back(oar1);

	Object* oar2{ new Object{ "sub1_oar2" } };
	rc = new RenderingComponent{ oar2 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	oar2->set_position(Vec3f{ 1.5f, 0.0f, 0.37f });
	oar2->set_scale(Vec3f{ 1.5f, 0.2f, 0.1f });
	oar2->set_parent(sub1_oar_controller);

	m_objects.push_back(oar2);

	Object* oar3{ new Object{ "sub1_oar3" } };
	rc = new RenderingComponent{ oar3 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	oar3->set_position(Vec3f{ -1.5f, 0.0f, -0.37f });
	oar3->set_scale(Vec3f{ 1.5f, 0.2f, 0.1f });
	oar3->set_parent(sub1_oar_controller);

	m_objects.push_back(oar3);

	Object* oar4{ new Object{ "sub1_oar4" } };
	rc = new RenderingComponent{ oar4 };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	rc->set_material(mat2);
	oar4->set_position(Vec3f{ -1.5f, 0.0f, 0.37f });
	oar4->set_scale(Vec3f{ 1.5f, 0.2f, 0.1f });
	oar4->set_parent(sub1_oar_controller);

	m_objects.push_back(oar4);

	// -----------------------------------------------------------------------------------------------------------------

	float win_x = WindowingService::get_window(0)->get_size().x;
	float win_y = WindowingService::get_window(0)->get_size().y;

	Object* cam{ new Object{ "camera1" } };
	CameraComponent* cc{ new CameraComponent{ cam, MathUtils::to_radians(60.0f), win_x, win_y, 0.1f, 1000.0f } };

	//cam->set_position(Vec3f(0.0f, 0.0f, -20.0f));

	m_objects.push_back(cam);

	Object* cam2{ new Object{ "camera2" } };
	CameraComponent* cc2{ new CameraComponent{ cam2, MathUtils::to_radians(60.0f), win_x, win_y, 0.1f, 1000.0f } };

	cam2->set_position(Vec3f(0.0f, 0.0f, -3.0f));

	m_objects.push_back(cam2);

	Object* light1{ new Object{ "light1" } };

	LightComponent* lc1{ new LightComponent{ light1 } };

	LightDesc light_desc;
	light_desc.ambient_intensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	light_desc.diffuse_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc.specular_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc.flags = Vec4ui{ 1, 1, 0, 0 };
	light_desc.attenuation = Vec3f{ 1.0f, 0.0f, 0.0f };
	light_desc.light_projection_matrix = MathUtils::perspective_lh(light_desc.light_projection_matrix, MathUtils::to_radians(60.0), 2048, 2048, 5.0f, 50.0f);

	lc1->set_light_description(light_desc);

	light1->set_position(Vec3f{ -10.0f, 10.0, -10.0f });

	m_objects.push_back(light1);

	Object* light2{ new Object{ "light2" } };

	LightComponent* lc2{ new LightComponent{ light2 } };

	LightDesc light_desc2;
	light_desc2.ambient_intensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	light_desc2.diffuse_intensity = Vec4f{ 1.0f, 1.0f, 0.0f, 1.0f };
	light_desc2.specular_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc2.flags = Vec4ui{ 0, 1, 0, 0 };
	light_desc2.attenuation = Vec3f{ 1.0f, 0.0f, 0.0f };
	light_desc2.spot_cutoff = 30.0f;
	light_desc2.spot_exponent = 50.0f;
	light_desc2.spot_direction = Vec3f{ 1.0f, 0.0f, 1.0f };
	light_desc2.light_projection_matrix = MathUtils::perspective_lh(light_desc2.light_projection_matrix, MathUtils::to_radians(60.0), 2048, 2048, 5.0f, 50.0f);

	lc2->set_light_description(light_desc2);

	light2->set_position(Vec3f{ -10.0f, 0.0, -10.0f});

	m_objects.push_back(light2);

	Object* light3{ new Object{ "light3" } };

	LightComponent* lc3{ new LightComponent{ light3 } };

	LightDesc light_desc3;
	light_desc3.ambient_intensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	light_desc3.diffuse_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc3.specular_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc3.flags = Vec4ui{ 0, 1, 0, 0 };
	light_desc3.attenuation = Vec3f{ 1.0f, 0.0f, 0.0f };
	light_desc3.spot_cutoff = 30.0f;
	light_desc3.spot_exponent = 50.0f;
	light_desc3.spot_direction = Vec3f{ -1.0f, 0.0f, 1.0f };
	light_desc3.light_projection_matrix = MathUtils::perspective_lh(light_desc3.light_projection_matrix, MathUtils::to_radians(60.0), 2048, 2048, 5.0f, 50.0f);

	lc3->set_light_description(light_desc3);

	light3->set_position(Vec3f{ 10.0f, 0.0, -10.0f });

	m_objects.push_back(light3);

	Object* light4{ new Object{ "light4" } };

	LightComponent* lc4{ new LightComponent{ light4 } };

	LightDesc light_desc4;
	light_desc4.ambient_intensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	light_desc4.diffuse_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc4.specular_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc4.flags = Vec4ui{ 0, 1, 0, 0 };
	light_desc4.attenuation = Vec3f{ 1.0f, 0.0f, 0.0f };
	light_desc4.spot_cutoff = 30.0f;
	light_desc4.spot_exponent = 50.0f;
	light_desc4.spot_direction = Vec3f{ 0.0f, 0.0f, -1.0f };
	light_desc4.light_projection_matrix = MathUtils::perspective_lh(light_desc3.light_projection_matrix, MathUtils::to_radians(60.0), 2048, 2048, 2.0f, 50.0f);

	lc4->set_light_description(light_desc4);

	light4->set_position(Vec3f{ 0.0f, 0.0, 10.0f });

	m_objects.push_back(light4);

	Window* win{ WindowingService::get_window(0) };
	m_color_pass_rt.create(win->get_size());
	
	for(int i = 0; i < 4; ++i) {
		m_depth_pass_rts[i].create(Vec2f{ 2048, 2048 });
	}
}

void MainScene::on_key_down(unsigned char key, int x, int y) noexcept
{
}

void MainScene::on_key_up(unsigned char key, int x, int y) noexcept
{
	switch (key) {
	case '1':
		EngineContext::get_camera_system()->set_active_camera("camera1");
		break;
	case '2':
		EngineContext::get_camera_system()->set_active_camera("camera2");
		break;
	case 'Z':
		EngineContext::get_light_system()->toggle_light("light1");
		break;
	case 'X':
		EngineContext::get_light_system()->toggle_light("light2");
		break;
	case 'C':
		EngineContext::get_light_system()->toggle_light("light3");
		break;
	case 'V':
		EngineContext::get_light_system()->toggle_light("light4");
		break;
	case 'W':
		wireframe = !wireframe;
		break;
	default:
		break;
	}
}

static int prev_x, prev_y;
static int bnstate[8];

void MainScene::on_mouse_motion(int x, int y) noexcept
{
	int dx = x - prev_x;
	int dy = y - prev_y;
	prev_x = x;
	prev_y = y;

	if(!dx && !dy) return;

	if(bnstate[0]) {
		EngineContext::get_camera_system()->get_active_camera()->set_euler_angles(Vec3f{ dy * 0.5, dx * 0.5, 0 });

		cam_theta += dx * 0.5;
		cam_phi += dy * 0.5;
		if(cam_phi < -90) cam_phi = -90;
		if(cam_phi > 90) cam_phi = 90;
	}
}

void MainScene::on_mouse_click(int button, bool state, int x, int y)
{
	prev_x = x;
	prev_y = y;
	bnstate[button] = state;
}

void MainScene::update(float delta_time, long time) noexcept
{
	sub1_oar_root->set_euler_angles(Vec3f{ 0, cos(time / 300.0f) * 3.0f, sin(time / 300.0f) * 2.0f });
	sub1_root->set_position(Vec3f{ cos(MathUtils::to_radians(time / 100.0f)) * 4.0f, 0.0f, sin(MathUtils::to_radians(time / 100.0f)) * 4.0f });

	Scene::update(delta_time, time);

	CameraSystem* camera_system{ EngineContext::get_camera_system() };

	camera_system->process(m_objects, delta_time);

	LightSystem* light_system{ EngineContext::get_light_system() };

	light_system->process(m_objects, delta_time);

	D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };
	ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

	std::vector<LightDesc> lights{ light_system->get_active_light_descriptions() };

	D3D11_MAPPED_SUBRESOURCE lms;
	device_context->Map(m_light_structured_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lms);
	memcpy(lms.pData, &lights[0], lights.size() * sizeof(LightDesc));
	device_context->Unmap(m_light_structured_buffer.Get(), 0);
}

void MainScene::draw() const noexcept
{
	depth_pass();
	color_pass();
	display_to_screen();
}
