#include "main_scene.h"
#include "internal/engine_context.h"
#include "mesh_utils.h"
#include "resource_manager.h"
#include "camera_component.h"
#include "../GlacierEngine/rendering/include/render_state_manager.h"
#include "../GlacierEngine/windowing/include/windowing_service.h"
#include "shader_program_manager.h"
#include "emitter_conponent.h"
#include "../GlacierEngine/rendering/include/d3d/D3D11_texture.h"
#include "keypress_message.h"
#include "camera_keyboard_input_component.h"
#include "drebel_sub.h"
#include "path_component.h"

using namespace Glacier;

static bool wireframe{ false };

struct ColorPassUniformBuffer {
	Mat4f MVP;
	Mat4f MV;
	Mat4f M;
	Mat4f V;
	Mat4f P;
	Mat4f ITMV;
	Mat4f texture_matrix;
	Vec4f diffuse;
	Vec4f specular;
};

struct DepthPassUniformBuffer {
	Mat4f MVP;
};

struct ParticleUniformBuffer {
	Mat4f MVP;
	Vec4f diffuse;
};

struct SkyboxUniformBuffer {
	Mat4f VP;
	Mat4f ITV;
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

	float clear_color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };

	RenderStateManager::set(RenderStateType::RS_CULL_FRONT);

	for (int i = 0; i < lights.size(); ++i) {
		m_depth_pass_rts[i].bind(RenderTargetBindType::DEPTH);
		m_depth_pass_rts[i].clear(clear_color);

		if (lights[i].flags.y == true) {
			ShaderProgramManager::get("depth_pass_sdrprog")->bind();

			for (auto rendering_component : rendering_components) {

				if (rendering_component->get_mesh() && rendering_component->should_draw() && rendering_component->casts_shadows()) {

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
					} else {
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
//	float clear_color[4]{ 0.0470588235294118f, 0.3019607843137255f, 0.4117647058823529f, 1.0f };
	float clear_color[4]{ 0.0f, 0.0f, 0.0f, 1.0f };

	m_color_pass_rt.bind(RenderTargetBindType::COLOR_AND_DEPTH);
	m_color_pass_rt.clear(clear_color);

	if (wireframe) {
		RenderStateManager::set(RenderStateType::RS_DRAW_WIRE);
	}

	D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };
	ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(WindowingService::get_window(0)->get_size().x);
	viewport.Height = static_cast<float>(WindowingService::get_window(0)->get_size().y);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	device_context->RSSetViewports(1, &viewport);

	render_skybox();

	std::vector<RenderingComponent*> rendering_components;
	std::vector<EmitterComponent*> emitter_components;

	for (auto object : m_objects) {
		RenderingComponent* rc{ static_cast<RenderingComponent*>(object->get_component("co_rendering")) };
		EmitterComponent* ec{ static_cast<EmitterComponent*>(object->get_component("co_emitter")) };

		if (rc) {
			rendering_components.push_back(rc);
		}

		if (ec) {
			emitter_components.push_back(ec);
		}
	}

	std::sort(rendering_components.begin(),
	          rendering_components.end(),
	          [](auto a, auto b) {
		          return a->get_material().diffuse.w > b->get_material().diffuse.w;
	          });

	ShaderProgramManager::get("color_pass_sdrprog")->bind();

	device_context->PSSetShaderResources(4, 1, m_light_srv.GetAddressOf());
	device_context->PSSetSamplers(0, 1, m_sampler_linear_wrap.GetAddressOf());
	device_context->PSSetSamplers(1, 1, m_sampler_shadow_comparison.GetAddressOf());

	std::vector<ID3D11ShaderResourceView*> depth_textures;
	for (int i = 0; i < 4; ++i) {
		depth_textures.push_back(m_depth_pass_rts[i].get_depth_attachment());
	}

	device_context->PSSetShaderResources(5, 4, depth_textures.data());

	CameraSystem* camera_system{ EngineContext::get_camera_system() };

	Mat4f view{ camera_system->get_active_camera_view_matrix() };

	Mat4f projection{ camera_system->get_active_camera_projection_matrix() };

	for (auto rendering_component : rendering_components) {

		if (rendering_component->get_mesh() && rendering_component->should_draw()) {

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
			uniforms.texture_matrix = MathUtils::transpose(material.texture_matrix);
			uniforms.diffuse = material.diffuse;
			uniforms.specular = material.specular;

			if (material.textures[TEX_DIFFUSE]) {
				material.textures[TEX_DIFFUSE]->bind();
			} else {
				ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyDiff.jpg")->bind();
			}

			if (material.textures[TEX_SPECULAR]) {
				material.textures[TEX_SPECULAR]->bind();
			} else {
				ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummySpec.jpg")->bind();
			}

			if (material.textures[TEX_NORMAL]) {
				material.textures[TEX_NORMAL]->bind();
			} else {
				ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyNorm.png")->bind();
			}

			D3D11_MAPPED_SUBRESOURCE ms;

			device_context->Map(m_color_pass_uniform_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			memcpy(ms.pData, &uniforms, sizeof(ColorPassUniformBuffer));
			device_context->Unmap(m_color_pass_uniform_buffer.Get(), 0);

			device_context->VSSetConstantBuffers(0, 1, m_color_pass_uniform_buffer.GetAddressOf());
			device_context->PSSetConstantBuffers(0, 1, m_color_pass_uniform_buffer.GetAddressOf());

			Mesh* mesh{ rendering_component->get_mesh() };

			RenderStateManager::set(material.blend_state);

			mesh->get_vbo()->bind();

			if (mesh->get_index_count()) {
				mesh->get_ibo()->bind();
				mesh->get_ibo()->draw();
			} else {
				mesh->get_vbo()->draw();
			}

			RenderStateManager::set(RenderStateType::BS_BLEND_DISSABLED);
		}
	}


	//Draw particles. -----------------------------------------------------------------------------------------------
	RenderStateManager::set(RenderStateType::DSS_DEPTH_MASK_0);
	ShaderProgramManager::get("particles_sdrprog")->bind();
	for (auto emitter : emitter_components) {
		Material mat{ emitter->get_material() };

		mat.textures[TEX_DIFFUSE]->bind();
		device_context->PSSetSamplers(0, 1, m_sampler_linear_wrap.GetAddressOf());

		RenderStateManager::set(mat.blend_state);
		auto particles = emitter->get_particles();

		for (auto particle : particles) {
			Mat4f model;
			model = MathUtils::translate(model, particle.position);
			model = MathUtils::scale(model, Vec3f{ particle.size });

			view[0][0] = 1.0f;
			view[0][1] = 0.0f;
			view[0][2] = 0.0f;

			view[1][0] = 0.0f;
			view[1][1] = 1.0f;
			view[1][2] = 0.0f;

			view[2][0] = 0.0f;
			view[2][1] = 0.0f;
			view[2][2] = 1.0f;

			Mat4f MVP{ projection * view * model };

			ParticleUniformBuffer uniforms;
			uniforms.MVP = MathUtils::transpose(MVP);
			uniforms.diffuse = particle.color;

			D3D11_MAPPED_SUBRESOURCE ms;

			device_context->Map(m_particle_uniform_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			memcpy(ms.pData, &uniforms, sizeof(ParticleUniformBuffer));
			device_context->Unmap(m_particle_uniform_buffer.Get(), 0);

			device_context->VSSetConstantBuffers(0, 1, m_particle_uniform_buffer.GetAddressOf());
			device_context->PSSetConstantBuffers(0, 1, m_particle_uniform_buffer.GetAddressOf());

			Mesh* mesh = emitter->get_mesh();

			mesh->get_vbo()->bind();

			if (mesh->get_index_count()) {
				mesh->get_ibo()->bind();
				mesh->get_ibo()->draw();
			} else {
				mesh->get_vbo()->draw();
			}
		}

	}
	RenderStateManager::set(RenderStateType::BS_BLEND_DISSABLED);
	RenderStateManager::set(RenderStateType::DSS_DEPTH_MASK_1);
	//-------------------------------------------------------------------------------------------------------------


	render_globe();

	std::vector<ID3D11ShaderResourceView*> null_srvs{ nullptr, nullptr, nullptr, nullptr };

	device_context->PSSetShaderResources(5, 4, null_srvs.data());

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

void MainScene::render_globe() const noexcept
{
	RenderingComponent* rc{ dynamic_cast<RenderingComponent*>(m_globe->get_component("co_rendering")) };

	if (rc) {

		if (rc->get_mesh() && rc->should_draw()) {
			ShaderProgramManager::get("color_pass_sdrprog")->bind();
			D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };
			ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

			device_context->PSSetShaderResources(4, 1, m_light_srv.GetAddressOf());
			device_context->PSSetSamplers(0, 1, m_sampler_linear_wrap.GetAddressOf());
			device_context->PSSetSamplers(1, 1, m_sampler_shadow_comparison.GetAddressOf());

			std::vector<ID3D11ShaderResourceView*> depth_textures;
			for (int i = 0; i < 4; ++i) {
				depth_textures.push_back(m_depth_pass_rts[i].get_depth_attachment());
			}

			device_context->PSSetShaderResources(5, 4, depth_textures.data());

			CameraSystem* camera_system{ EngineContext::get_camera_system() };

			Mat4f view{ camera_system->get_active_camera_view_matrix() };

			Mat4f projection{ camera_system->get_active_camera_projection_matrix() };

			Mat4f model{ rc->get_xform() };

			Mat4f MVP{ projection * view * model };
			Mat4f MV{ view * model };
			Mat4f ITMV{ MathUtils::transpose(MathUtils::inverse(MV)) };

			Material material{ rc->get_material() };

			ColorPassUniformBuffer uniforms;
			uniforms.MVP = MathUtils::transpose(MVP);
			uniforms.MV = MathUtils::transpose(MV);
			uniforms.M = MathUtils::transpose(model);
			uniforms.V = MathUtils::transpose(view);
			uniforms.P = MathUtils::transpose(projection);
			uniforms.ITMV = MathUtils::transpose(ITMV);
			uniforms.diffuse = material.diffuse;
			uniforms.specular = material.specular;

			if (material.textures[TEX_DIFFUSE]) {
				material.textures[TEX_DIFFUSE]->bind();
			} else {
				ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyDiff.jpg")->bind();
			}

			if (material.textures[TEX_SPECULAR]) {
				material.textures[TEX_SPECULAR]->bind();
			} else {
				ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummySpec.jpg")->bind();
			}

			if (material.textures[TEX_NORMAL]) {
				material.textures[TEX_NORMAL]->bind();
			} else {
				ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyNorm.png")->bind();
			}

			D3D11_MAPPED_SUBRESOURCE ms;

			device_context->Map(m_color_pass_uniform_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			memcpy(ms.pData, &uniforms, sizeof(ColorPassUniformBuffer));
			device_context->Unmap(m_color_pass_uniform_buffer.Get(), 0);

			device_context->VSSetConstantBuffers(0, 1, m_color_pass_uniform_buffer.GetAddressOf());
			device_context->PSSetConstantBuffers(0, 1, m_color_pass_uniform_buffer.GetAddressOf());

			Mesh* mesh{ rc->get_mesh() };

			RenderStateManager::set(material.blend_state);

			mesh->get_vbo()->bind();

			if (mesh->get_index_count()) {
				mesh->get_ibo()->bind();
				mesh->get_ibo()->draw();
			} else {
				mesh->get_vbo()->draw();
			}

			RenderStateManager::set(RenderStateType::BS_BLEND_DISSABLED);
		}
	}
}

void MainScene::render_skybox() const noexcept
{
	RenderingComponent* rc{ dynamic_cast<RenderingComponent*>(m_skybox->get_component("co_rendering")) };

	if (rc) {
		if (rc->get_mesh() && rc->should_draw()) {
			D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };
			ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

			RenderStateManager::set(RenderStateType::DSS_DEPTH_MASK_0);
			ShaderProgramManager::get("skybox_sdrprog")->bind();

			device_context->PSSetSamplers(0, 1, m_sampler_linear_wrap.GetAddressOf());

			CameraSystem* camera_system{ EngineContext::get_camera_system() };

			Mat4f view{ camera_system->get_active_camera_view_matrix() };
			view[3][0] = 0.0f;
			view[3][1] = 0.0f;
			view[3][2] = 0.0f;
			view[3][3] = 1.0f;

			view[0][3] = 0.0f;
			view[1][3] = 0.0f;
			view[2][3] = 0.0f;

			Mat4f projection{ camera_system->get_active_camera_projection_matrix() };

			Mat4f VP{ projection * view};
			Mat4f ITV{ view };

			SkyboxUniformBuffer uniforms;
			uniforms.VP = MathUtils::transpose(VP);
			uniforms.ITV = MathUtils::transpose(ITV);

			Material material{ rc->get_material() };
			if (material.textures[TEX_DIFFUSE]) {
				material.textures[TEX_DIFFUSE]->bind();
			}
			else {
				ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyDiff.jpg")->bind();
			}

			D3D11_MAPPED_SUBRESOURCE ms;
			device_context->Map(m_skybox_uniform_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			memcpy(ms.pData, &uniforms, sizeof(SkyboxUniformBuffer));
			device_context->Unmap(m_skybox_uniform_buffer.Get(), 0);

			device_context->VSSetConstantBuffers(0, 1, m_skybox_uniform_buffer.GetAddressOf());

			Mesh* mesh{ rc->get_mesh() };

			//RenderStateManager::set(material.blend_state);

			mesh->get_vbo()->bind();

			if (mesh->get_index_count()) {
				mesh->get_ibo()->bind();
				mesh->get_ibo()->draw();
			}
			else {
				mesh->get_vbo()->draw();
			}

			RenderStateManager::set(RenderStateType::DSS_DEPTH_MASK_1);
		}
	}
}

void MainScene::setup_lights() noexcept
{
	Object* light1{ new Object{ "light1" } };

	LightComponent* lc1{ new LightComponent{ light1 } };

	LightDesc light_desc;
	light_desc.ambient_intensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	light_desc.diffuse_intensity = Vec4f{ 0.7f, 0.7f, 0.7f, 1.0f };
	light_desc.specular_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc.flags = Vec4ui{ 1, 1, 0, 0 };
	light_desc.attenuation = Vec3f{ 1.0f, 0.0f, 0.0f };
	light_desc.light_projection_matrix = MathUtils::perspective_lh(MathUtils::to_radians(60.0), 2048, 2048, 5.0f, 46.0f);
	lc1->set_light_description(light_desc);
	light1->set_position(Vec3f{ 0.0f, 30.0, 0.0f });
	light1->setup();
	m_objects.push_back(light1);

	Object* light2{ new Object{ "light2" } };
	LightComponent* lc2{ new LightComponent{ light2 } };
	LightDesc light_desc2;
	light_desc2.ambient_intensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	light_desc2.diffuse_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc2.specular_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc2.flags = Vec4ui{ 0, 1, 0, 0 };
	light_desc2.attenuation = Vec3f{ 1.0f, 0.0f, 0.0f };
	light_desc2.spot_cutoff = 20.0f;
	light_desc2.spot_exponent = 90.0f;
	light_desc2.spot_direction = Vec3f{ 0.09f, -0.1f, 0.09f };
	light_desc2.light_projection_matrix = MathUtils::perspective_lh(MathUtils::to_radians(60.0), 2048, 2048, 5.0f, 50.0f);
	lc2->set_light_description(light_desc2);
	light2->set_position(Vec3f{ -10.0f, 10.0, -10.0f });
	light2->setup();
	m_objects.push_back(light2);

	Object* light3{ new Object{ "light3" } };
	LightComponent* lc3{ new LightComponent{ light3 } };
	LightDesc light_desc3;
	light_desc3.ambient_intensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	light_desc3.diffuse_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc3.specular_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc3.flags = Vec4ui{ 0, 1, 0, 0 };
	light_desc3.attenuation = Vec3f{ 1.0f, 0.0f, 0.0f };
	light_desc3.spot_cutoff = 20.0f;
	light_desc3.spot_exponent = 90.0f;
	light_desc3.spot_direction = Vec3f{ -0.09f, -0.1f, 0.09f };
	light_desc3.light_projection_matrix = MathUtils::perspective_lh(MathUtils::to_radians(60.0), 2048, 2048, 5.0f, 50.0f);
	lc3->set_light_description(light_desc3);
	light3->set_position(Vec3f{ 10.0f, 10.0, -10.0f });
	light3->setup();
	m_objects.push_back(light3);

	Object* light4{ new Object{ "light4" } };
	LightComponent* lc4{ new LightComponent{ light4 } };
	LightDesc light_desc4;
	light_desc4.ambient_intensity = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
	light_desc4.diffuse_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc4.specular_intensity = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	light_desc4.flags = Vec4ui{ 0, 1, 0, 0 };
	light_desc4.attenuation = Vec3f{ 1.0f, 0.0f, 0.0f };
	light_desc4.spot_cutoff = 20.0f;
	light_desc4.spot_exponent = 90.0f;
	light_desc4.spot_direction = Vec3f{ 0.0f, -0.1f, -0.1f };
	light_desc4.light_projection_matrix = MathUtils::perspective_lh(MathUtils::to_radians(60.0), 2048, 2048, 2.0f, 50.0f);
	lc4->set_light_description(light_desc4);
	light4->set_position(Vec3f{ 0.0f, 10.0, 10.0f });
	light4->setup();
	m_objects.push_back(light4);
}

void MainScene::setup_cameras() noexcept
{
	float win_x = WindowingService::get_window(0)->get_size().x;
	float win_y = WindowingService::get_window(0)->get_size().y;

	Object* cam{ new Object{ "camera1" } };
	CameraComponent* cc{ new CameraComponent{ cam, MathUtils::to_radians(60.0f), win_x, win_y, 0.1f, 1000.0f } };
	CameraKeyboardInputComponent* input_comp{ new CameraKeyboardInputComponent{ cam } };
	input_comp->set_movement_speed(30.0f);
	input_comp->set_rotation_speed(180.0f);
	cam->set_position(Vec3f(0.0f, 0.0f, -80.0f));
	cam->setup();
	m_objects.push_back(cam);

	Object* cam2{ new Object{ "camera2" } };
	CameraComponent* cc2{ new CameraComponent{ cam2, MathUtils::to_radians(60.0f), win_x, win_y, 0.1f, 1000.0f } };
	input_comp = new CameraKeyboardInputComponent{ cam2 };
	input_comp->set_rotation_speed(90.0f);
	cam2->set_parent(m_drebel);
	cam2->set_position(Vec3f(0.0f, 0.0f, -1.0f));
	cam2->setup();
	m_objects.push_back(cam2);
}

void MainScene::setup_d3d() noexcept
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

	ParticleUniformBuffer p_uniforms;
	ZeroMemory(&cb_desc, sizeof(cb_desc));
	cb_desc.ByteWidth = sizeof(p_uniforms);
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;

	res = device->CreateBuffer(&cb_desc, nullptr, m_particle_uniform_buffer.ReleaseAndGetAddressOf());

	if (FAILED(res)) {
		std::cerr << "Renderer initialization failed: Uniform buffer creation failed." << std::endl;
	}

	SkyboxUniformBuffer sky_uniforms;
	ZeroMemory(&cb_desc, sizeof(cb_desc));
	cb_desc.ByteWidth = sizeof(sky_uniforms);
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;

	res = device->CreateBuffer(&cb_desc, nullptr, m_skybox_uniform_buffer.ReleaseAndGetAddressOf());

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
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
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

	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1;
	samplerDesc.BorderColor[1] = 1;
	samplerDesc.BorderColor[2] = 1;
	samplerDesc.BorderColor[3] = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;

	res = device->CreateSamplerState(&samplerDesc, m_sampler_shadow_comparison.ReleaseAndGetAddressOf());

	if (FAILED(res)) {
		std::cerr << "Linear Texture Wrap sampler creation failed!" << std::endl;
	}
}

void MainScene::initialize()
{
	setup_d3d();

	Mesh* m{ MeshUtils::generate_uv_sphere(1.0f, 60, 60) };
	ResourceManager::register_resource(m, L"sphere");

	m = MeshUtils::generate_cube(1.0f);
	ResourceManager::register_resource(m, L"cube");

	m = MeshUtils::generate_cube(1.0f, VertexWinding::ANTICLOCKWISE);
	ResourceManager::register_resource(m, L"flipped_cube");

	m = MeshUtils::generate_plane_xy(1.0f);
	ResourceManager::register_resource(m, L"plane");

	ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyDiff.jpg")->set_texture_type(TEX_DIFFUSE);
	ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummySpec.jpg")->set_texture_type(TEX_SPECULAR);
	ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyNorm.png")->set_texture_type(TEX_NORMAL);
	ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"sky.dds")->set_texture_type(TEX_DIFFUSE);

	Material mat;
	mat.diffuse = Vec4f{ 0.0470588235294118f, 0.3019607843137255f, 0.4117647058823529f, 0.5f };
	mat.specular = Vec4f{ 0.5f, 0.5f, 0.5f, 128.0f };
	mat.blend_state = RenderStateType::BS_BLEND_ALPHA;

	m_globe = new Object{ "globe" };
	RenderingComponent* rc{ new RenderingComponent{ m_globe } };
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat);
	rc->set_casts_shadows(false);
	m_globe->set_position(Vec3f{ 0.0f, 0.0, 0.0f });
	m_globe->set_scale(Vec3f{ 20.0f, 20.0f, 20.0f });
	m_globe->calculate_xform();
	m_globe->setup();

	m_skybox = new Object{ "skybox" };
	rc = new RenderingComponent{ m_skybox };
	rc->set_mesh(ResourceManager::get<Mesh>(L"flipped_cube"));
	Material skybox_mat;
	skybox_mat.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"sky.dds");
	rc->set_material(skybox_mat);
	m_skybox->setup();

	Object* obj = new Object{ "ground" };
	rc = new RenderingComponent{ obj };
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	mat.diffuse = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
	mat.specular = Vec4f{ 0.2f, 0.2f, 0.2f, 30.0f };
	mat.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"mars_diff.png");
	mat.textures[TEX_SPECULAR] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"mars_spec.png");
	mat.textures[TEX_NORMAL] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"mars_norm.png");
	mat.textures[TEX_DIFFUSE]->set_texture_type(TEX_DIFFUSE);
	mat.textures[TEX_SPECULAR]->set_texture_type(TEX_SPECULAR);
	mat.textures[TEX_NORMAL]->set_texture_type(TEX_NORMAL);
	mat.texture_matrix = MathUtils::scale(mat.texture_matrix, Vec3f{ 30.0f, 30.0f , 0.0f });
	rc->set_material(mat);
	rc->set_casts_shadows(false);
	obj->set_position(Vec3f{ 0.0f, -10.0, 0.0f });
	obj->set_scale(Vec3f{ 2000.0f, 0.1f, 2000.0f });
	obj->setup();
	m_objects.push_back(obj);

	// Submarine 1 creation -------------------------------------------------------------------------------------------
	m_drebel = new DrebelSubmarine{ "drebel_sub", this };
	m_drebel->set_scale(Vec3f{ 5.0f, 5.0f, 5.0f });
	PathComponent* pc{ new PathComponent{ m_drebel } };
	pc->add_keyframe(Vec3f{ 10, 0, 0 }, 0);
	pc->add_keyframe(Vec3f{ 8, 0, 2 }, 2000);
	pc->add_keyframe(Vec3f{ 5, 2, 4 }, 4000);
	pc->add_keyframe(Vec3f{ 2, 4, 8 }, 6000);
	pc->add_keyframe(Vec3f{ -5, 2, 4 }, 8000);
	pc->add_keyframe(Vec3f{ -2, 0, 0 }, 10000);
	pc->add_keyframe(Vec3f{ 5, 0, 0 }, 12000);
	pc->add_keyframe(Vec3f{ 10, 0, 0 }, 14000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	m_drebel->setup();
	// -----------------------------------------------------------------------------------------------------------------

	setup_cameras();

	setup_lights();

	Object* emitter{ new Object{ "emmiter1" } };
	EmitterComponent* ec{ new EmitterComponent{ emitter } };
	ec->set_lifespan(6.0);
	ec->set_max_particles(1000);
	ec->set_spawn_rate(20.0);
	ec->set_active(true);
	ec->set_particle_size(0.3f);
	ec->set_spawn_radius(13.0f);
	ec->set_velocity(Vec3f{ 0.0f, 0.0f, 0.0f });
	ec->set_velocity_range(0.05f);
	ec->set_external_force(Vec3f{ 0.0f, 0.1f, 0.0f });
	ec->set_mesh(ResourceManager::get<Mesh>(L"plane"));
	ec->set_start_color(Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f });
	ec->set_end_color(Vec4f{ 1.0f, 1.0f, 1.0f, 0.3f });
	Material p_mat;
	p_mat.blend_state = RenderStateType::BS_BLEND_ADDITIVE;
	p_mat.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"bubble10.png");
	p_mat.textures[TEX_DIFFUSE]->set_texture_type(TEX_DIFFUSE);
	ec->set_material(p_mat);
	//emitter->set_position(Vec3f{ 0.0f, -18.0f, 0.0f });
	emitter->setup();
	m_objects.push_back(emitter);

	Window* win{ WindowingService::get_window(0) };
	m_color_pass_rt.create(win->get_size());

	for (int i = 0; i < 4; ++i) {
		m_depth_pass_rts[i].create(Vec2f{ 2048, 2048 });
	}
}

void MainScene::on_key_down(unsigned char key, int x, int y) noexcept
{
	MessageContainer msg{ new KeypressMessage{ key, x, y, true } };
	Scene::on_message(msg);
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
	case '0':
		wireframe = !wireframe;
		break;
	default:
		break;
	}

	MessageContainer msg{ new KeypressMessage{ key, x, y, false } };
	Scene::on_message(msg);
}

static int prev_x, prev_y;
static int bnstate[8];

void MainScene::on_mouse_motion(int x, int y) noexcept
{
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
