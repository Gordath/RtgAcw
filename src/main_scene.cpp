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
#include "AntTweakBar.h"

using namespace Glacier;

static bool wireframe{false};
static float simulation_speed{1.0f};
static float fps{0.0f};
static bool shadows{true};
static long dt_ms{0};

static float fresnel_power{1.9f};
static float fresnel_bias{0.12f};

static LightDesc* directional_desc{nullptr};
static LightDesc* spotlight1_desc{nullptr};
static LightDesc* spotlight2_desc{nullptr};
static LightDesc* spotlight3_desc{nullptr};

static LightDesc default_directional;
static LightDesc default_spotlight1;
static LightDesc default_spotlight2;
static LightDesc default_spotlight3;

struct ColorPassUniformBuffer
{
	Mat4f MVP;
	Mat4f MV;
	Mat4f M;
	Mat4f V;
	Mat4f P;
	Mat4f ITMV;
	Mat4f texture_matrix;
	Vec4f diffuse;
	Vec4f specular;
	float fpower;
	float fbias;
	float pad;
	float pad1;
};

struct DepthPassUniformBuffer
{
	Mat4f MVP;
};

struct ParticleUniformBuffer
{
	Mat4f MVP;
	Vec4f diffuse;
};

struct SkyboxUniformBuffer
{
	Mat4f VP;
	Mat4f ITV;
};

static void TW_CALL toggle_spotlight_one(void* cient_data) // parasoft-suppress  CODSTA-CPP-53 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost." // parasoft-suppress  MISRA2004-16_7 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost."
{
	EngineContext::get_light_system()->toggle_light("light2");
}

static void TW_CALL toggle_spotlight_two(void* client_data) // parasoft-suppress  CODSTA-CPP-53 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost." // parasoft-suppress  MISRA2004-16_7 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost."
{
	EngineContext::get_light_system()->toggle_light("light3");
}

static void TW_CALL toggle_spotlight_three(void* client_data) // parasoft-suppress  MISRA2004-16_7 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost." // parasoft-suppress  CODSTA-CPP-53 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost."
{
	EngineContext::get_light_system()->toggle_light("light4");
}

static void TW_CALL toggle_directional_light(void* client_data) // parasoft-suppress  MISRA2004-16_7 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost." // parasoft-suppress  CODSTA-CPP-53 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost."
{
	EngineContext::get_light_system()->toggle_light("light1");
}

static void TW_CALL toggle_shadows(void* client_data) // parasoft-suppress  CODSTA-CPP-53 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost." // parasoft-suppress  MISRA2004-16_7 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost."
{
	shadows = !shadows;
}

static void TW_CALL toggle_wireframe(void* client_data) // parasoft-suppress  MISRA2004-16_7 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost." // parasoft-suppress  CODSTA-CPP-53 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost."
{
	wireframe = !wireframe;
}

static void TW_CALL activate_outer_camera(void* client_data) // parasoft-suppress  CODSTA-CPP-53 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost." // parasoft-suppress  MISRA2004-16_7 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost."
{
	EngineContext::get_camera_system()->set_active_camera("camera1");
}

static void TW_CALL activate_inner_camera(void* client_data) // parasoft-suppress  MISRA2004-16_7 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost."
{
	EngineContext::get_camera_system()->set_active_camera("camera2");
}

static void TW_CALL activate_inner_follow_camera(void* client_data) // parasoft-suppress  MISRA2004-16_7 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost." // parasoft-suppress  CODSTA-CPP-53 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost."
{
	EngineContext::get_camera_system()->set_active_camera("camera3");
}

static void TW_CALL reset_scene(void* client_data) // parasoft-suppress  MISRA2004-16_7 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost." // parasoft-suppress  CODSTA-CPP-53 "AntTweakBar's documentation specifies this exact signature for the callback function. Thats why this pointer is not cost."
{
	*directional_desc = default_directional;
	*spotlight1_desc = default_spotlight1;
	*spotlight2_desc = default_spotlight2;
	*spotlight3_desc = default_spotlight3;

	wireframe = false;
	simulation_speed = 1.0f;
	shadows = true;

	fresnel_power = 1.9f;
	fresnel_bias = 0.12f;
}

MainScene::~MainScene()
{
	TwTerminate();
}

void MainScene::depth_pass() const noexcept
{
	auto lights{EngineContext::get_light_system()->get_active_light_descriptions()};

	if (shadows)
	{
		D3D11Context* GAPI_context{EngineContext::get_GAPI_context()};
		ComPtr<ID3D11DeviceContext> device_context{GAPI_context->get_device_context()};

		std::vector<RenderingComponent*> rendering_components;

		for (auto object : get_objects())
		{
			RenderingComponent* rc{static_cast<RenderingComponent*>(object->get_component("co_rendering"))}; // parasoft-suppress  OOP-35 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable."

			if (rc)
			{
				rendering_components.push_back(rc);
			}
		}

		Vec4f clear_color{1.0f, 1.0f, 1.0f, 1.0f};

		RenderStateManager::set(RenderStateType::RS_CULL_FRONT);

		for (int i = 0; i < lights.size(); ++i)
		{
			m_depth_pass_rts[i].bind(RenderTargetBindType::DEPTH);
			m_depth_pass_rts[i].clear(&clear_color[0]);

			if (lights[i].flags.y == true)
			{
				ShaderProgramManager::get("depth_pass_sdrprog")->bind();

				for (auto rendering_component : rendering_components)
				{
					if (rendering_component->get_mesh() && rendering_component->should_draw() && rendering_component->casts_shadows())
					{
						Mat4f model{rendering_component->get_xform()};
						Mat4f light_view{lights[i].light_view_matrix};
						Mat4f light_projection{lights[i].light_projection_matrix};

						Mat4f MVP{light_projection * light_view * model};

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
						viewport.Width = static_cast<float>(m_depth_pass_rts[i].get_size().x);
						viewport.Height = static_cast<float>(m_depth_pass_rts[i].get_size().y);
						viewport.MinDepth = 0.0f;
						viewport.MaxDepth = 1.0f;

						device_context->RSSetViewports(1, &viewport);

						Mesh* mesh{rendering_component->get_mesh()};

						mesh->get_vbo()->bind();

						if (mesh->get_index_count())
						{
							mesh->get_ibo()->bind();
							mesh->get_ibo()->draw();
						}
						else
						{
							mesh->get_vbo()->draw();
						}
					}
				}
			}

			m_depth_pass_rts[i].unbind();
		}

		RenderStateManager::set(RenderStateType::RS_CULL_BACK);
	}
	else
	{
		Vec4f clear_color{1.0f, 1.0f, 1.0f, 1.0f};

		for (int i = 0; i < lights.size(); ++i)
		{
			m_depth_pass_rts[i].bind(RenderTargetBindType::DEPTH);
			m_depth_pass_rts[i].clear(&clear_color[0]);
			m_depth_pass_rts[i].unbind();
		}
	}
}

void MainScene::color_pass() const noexcept
{
	//	float clear_color[4]{ 0.0470588235294118f, 0.3019607843137255f, 0.4117647058823529f, 1.0f };
	Vec4f clear_color{0.0f, 0.0f, 0.0f, 1.0f};

	m_color_pass_rt.bind(RenderTargetBindType::COLOR_AND_DEPTH);
	m_color_pass_rt.clear(&clear_color[0]);

	if (wireframe)
	{
		RenderStateManager::set(RenderStateType::RS_DRAW_WIRE);
	}

	const D3D11Context* GAPI_context{EngineContext::get_GAPI_context()};
	const ComPtr<ID3D11DeviceContext> device_context{GAPI_context->get_device_context()};

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

	for (auto object : get_objects())
	{
		RenderingComponent* rc{static_cast<RenderingComponent*>(object->get_component("co_rendering"))}; // parasoft-suppress  OOP-35 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable."
		EmitterComponent* ec{static_cast<EmitterComponent*>(object->get_component("co_emitter"))}; // parasoft-suppress  OOP-35 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable."

		if (rc)
		{
			rendering_components.push_back(rc);
		}

		if (ec)
		{
			emitter_components.push_back(ec);
		}
	}

	std::sort(rendering_components.begin(),
	          rendering_components.end(),
	          [](auto a, auto b)
	          {
		          return a->get_material().diffuse.w > b->get_material().diffuse.w;
	          });

	ShaderProgramManager::get("color_pass_sdrprog")->bind();

	device_context->PSSetShaderResources(4, 1, m_light_srv.GetAddressOf());
	device_context->PSSetSamplers(0, 1, m_sampler_linear_wrap.GetAddressOf());
	device_context->PSSetSamplers(1, 1, m_sampler_shadow_comparison.GetAddressOf());

	std::vector<ID3D11ShaderResourceView*> depth_textures;
	for (int i = 0; i < 4; ++i)
	{
		depth_textures.push_back(m_depth_pass_rts[i].get_depth_attachment());
	}

	device_context->PSSetShaderResources(5, 4, depth_textures.data());

	CameraSystem* camera_system{EngineContext::get_camera_system()};

	Mat4f view{camera_system->get_active_camera_view_matrix()};

	Mat4f projection{camera_system->get_active_camera_projection_matrix()};

	for (auto rendering_component : rendering_components)
	{
		if (rendering_component->get_mesh() && rendering_component->should_draw())
		{
			Mat4f model{rendering_component->get_xform()};

			Mat4f MVP{projection * view * model};
			Mat4f MV{view * model};
			Mat4f ITMV{MathUtils::transpose(MathUtils::inverse(MV))};

			Material material{rendering_component->get_material()};

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

			if (material.textures[TEX_DIFFUSE])
			{
				material.textures[TEX_DIFFUSE]->bind();
			}
			else
			{
				ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyDiff.jpg")->bind();
			}

			if (material.textures[TEX_SPECULAR])
			{
				material.textures[TEX_SPECULAR]->bind();
			}
			else
			{
				ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummySpec.jpg")->bind();
			}

			if (material.textures[TEX_NORMAL])
			{
				material.textures[TEX_NORMAL]->bind();
			}
			else
			{
				ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyNorm.png")->bind();
			}

			D3D11_MAPPED_SUBRESOURCE ms;

			device_context->Map(m_color_pass_uniform_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			memcpy(ms.pData, &uniforms, sizeof(ColorPassUniformBuffer));
			device_context->Unmap(m_color_pass_uniform_buffer.Get(), 0);

			device_context->VSSetConstantBuffers(0, 1, m_color_pass_uniform_buffer.GetAddressOf());
			device_context->PSSetConstantBuffers(0, 1, m_color_pass_uniform_buffer.GetAddressOf());

			Mesh* mesh{rendering_component->get_mesh()};

			RenderStateManager::set(material.blend_state);

			mesh->get_vbo()->bind();

			if (mesh->get_index_count())
			{
				mesh->get_ibo()->bind();
				mesh->get_ibo()->draw();
			}
			else
			{
				mesh->get_vbo()->draw();
			}

			RenderStateManager::set(RenderStateType::BS_BLEND_DISSABLED);
		}
	}


	//Draw particles. -----------------------------------------------------------------------------------------------
	RenderStateManager::set(RenderStateType::DSS_DEPTH_MASK_0);
	ShaderProgramManager::get("particles_sdrprog")->bind();
	for (auto emitter : emitter_components)
	{
		Material mat{emitter->get_material()};

		mat.textures[TEX_DIFFUSE]->bind();
		device_context->PSSetSamplers(0, 1, m_sampler_linear_wrap.GetAddressOf());

		RenderStateManager::set(mat.blend_state);
		auto particles = emitter->get_particles();

		for (auto particle : particles)
		{
			Mat4f model;
			model = MathUtils::translate(model, particle.position);
			Mat4f MV{view * model};

			MV[0][0] = 1.0f;
			MV[0][1] = 0.0f;
			MV[0][2] = 0.0f;

			MV[1][0] = 0.0f;
			MV[1][1] = 1.0f;
			MV[1][2] = 0.0f;

			MV[2][0] = 0.0f;
			MV[2][1] = 0.0f;
			MV[2][2] = 1.0f;

			MV = MathUtils::scale(MV, Vec3f{particle.size});

			Mat4f MVP{projection * MV};

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

			if (mesh->get_index_count())
			{
				mesh->get_ibo()->bind();
				mesh->get_ibo()->draw();
			}
			else
			{
				mesh->get_vbo()->draw();
			}
		}
	}
	RenderStateManager::set(RenderStateType::BS_BLEND_DISSABLED);
	RenderStateManager::set(RenderStateType::DSS_DEPTH_MASK_1);
	//-------------------------------------------------------------------------------------------------------------

	std::vector<ID3D11ShaderResourceView*> null_srvs{nullptr, nullptr, nullptr, nullptr}; // parasoft-suppress  PB-23 "It's just a vector construction using an initializer list."

	device_context->PSSetShaderResources(5, 4, null_srvs.data());

	render_globe();

	TwDraw();

	m_color_pass_rt.unbind();
}

void MainScene::display_to_screen() const noexcept
{
	D3D11Context* ctx{EngineContext::get_GAPI_context()};

	ComPtr<ID3D11DeviceContext> dev_con{ctx->get_device_context()};

	dev_con->OMSetRenderTargets(1, ctx->get_address_of_render_target_view(), ctx->get_default_depth_stencil_view());

	Vec4f cl_col{0.0, 0.0, 0.0, 0.0};
	dev_con->ClearRenderTargetView(ctx->get_default_render_target_view(), &cl_col[0]);
	dev_con->ClearDepthStencilView(ctx->get_default_depth_stencil_view(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ShaderProgramManager::get("render_texture_sdrprog")->bind();

	ComPtr<ID3D11ShaderResourceView> srv{m_color_pass_rt.get_color_attachment()};
	dev_con->PSSetShaderResources(0, 1, srv.GetAddressOf());
	dev_con->PSSetSamplers(0, 1, m_sampler_linear_wrap.GetAddressOf());

	RenderStateManager::set(RenderStateType::RS_DRAW_SOLID);
	dev_con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	dev_con->Draw(4, 0);

	ID3D11ShaderResourceView* null_srv{nullptr};
	dev_con->PSSetShaderResources(0, 1, &null_srv);
}

void MainScene::render_globe() const noexcept
{
	RenderingComponent* rc{static_cast<RenderingComponent*>(m_globe->get_component("co_rendering"))}; // parasoft-suppress  OOP-29 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-49 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-35 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable."

	if (rc->get_mesh() && rc->should_draw())
	{
		ShaderProgramManager::get("globe_sdrprog")->bind();
		D3D11Context* GAPI_context{EngineContext::get_GAPI_context()};
		ComPtr<ID3D11DeviceContext> device_context{GAPI_context->get_device_context()};

		device_context->PSSetShaderResources(4, 1, m_light_srv.GetAddressOf());
		device_context->PSSetSamplers(0, 1, m_sampler_linear_wrap.GetAddressOf());

		CameraSystem* camera_system{EngineContext::get_camera_system()};

		Mat4f view{camera_system->get_active_camera_view_matrix()};

		Mat4f projection{camera_system->get_active_camera_projection_matrix()};

		Mat4f model{rc->get_xform()};

		Mat4f MVP{projection * view * model};
		Mat4f MV{view * model};
		Mat4f ITMV{MathUtils::transpose(MathUtils::inverse(MV))};

		Material material{rc->get_material()};

		ColorPassUniformBuffer uniforms;
		uniforms.MVP = MathUtils::transpose(MVP);
		uniforms.MV = MathUtils::transpose(MV);
		uniforms.M = MathUtils::transpose(model);
		uniforms.V = MathUtils::transpose(view);
		uniforms.P = MathUtils::transpose(projection);
		uniforms.ITMV = MathUtils::transpose(ITMV);
		uniforms.diffuse = material.diffuse;
		uniforms.specular = material.specular;
		uniforms.fpower = fresnel_power;
		uniforms.fbias = fresnel_bias;

		if (material.textures[TEX_DIFFUSE])
		{
			material.textures[TEX_DIFFUSE]->bind();
		}
		else
		{
			ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyDiff.jpg")->bind();
		}

		if (material.textures[TEX_SPECULAR])
		{
			material.textures[TEX_SPECULAR]->bind();
		}
		else
		{
			ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummySpec.jpg")->bind();
		}

		if (material.textures[TEX_NORMAL])
		{
			material.textures[TEX_NORMAL]->bind();
		}
		else
		{
			ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyNorm.png")->bind();
		}

		D3D11_MAPPED_SUBRESOURCE ms;

		device_context->Map(m_color_pass_uniform_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		memcpy(ms.pData, &uniforms, sizeof(ColorPassUniformBuffer));
		device_context->Unmap(m_color_pass_uniform_buffer.Get(), 0);

		device_context->VSSetConstantBuffers(0, 1, m_color_pass_uniform_buffer.GetAddressOf());
		device_context->PSSetConstantBuffers(0, 1, m_color_pass_uniform_buffer.GetAddressOf());

		Mesh* mesh{rc->get_mesh()};

		RenderStateManager::set(material.blend_state);

		mesh->get_vbo()->bind();

		if (mesh->get_index_count())
		{
			mesh->get_ibo()->bind();
			mesh->get_ibo()->draw();
		}
		else
		{
			mesh->get_vbo()->draw();
		}

		RenderStateManager::set(RenderStateType::BS_BLEND_DISSABLED);
	}
}

void MainScene::render_skybox() const noexcept
{
	RenderingComponent* rc{static_cast<RenderingComponent*>(m_skybox->get_component("co_rendering"))}; // parasoft-suppress  OOP-49 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-29 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable." // parasoft-suppress  OOP-35 "The whole compoment based architecture is based on casting from the base class Component to the appopriate component depending on the system that is processing it. The type of the component is already known based on a type comparison with the internal type member variable."

		if (rc->get_mesh() && rc->should_draw())
		{
			D3D11Context* GAPI_context{EngineContext::get_GAPI_context()};
			ComPtr<ID3D11DeviceContext> device_context{GAPI_context->get_device_context()};

			RenderStateManager::set(RenderStateType::DSS_DEPTH_MASK_0);
			ShaderProgramManager::get("skybox_sdrprog")->bind();

			device_context->PSSetSamplers(0, 1, m_sampler_linear_wrap.GetAddressOf());

			CameraSystem* camera_system{EngineContext::get_camera_system()};

			Mat4f view{camera_system->get_active_camera_view_matrix()};
			view[3][0] = 0.0f;
			view[3][1] = 0.0f;
			view[3][2] = 0.0f;
			view[3][3] = 1.0f;

			view[0][3] = 0.0f;
			view[1][3] = 0.0f;
			view[2][3] = 0.0f;

			Mat4f projection{camera_system->get_active_camera_projection_matrix()};

			Mat4f VP{projection * view};
			Mat4f ITV{view};

			SkyboxUniformBuffer uniforms;
			uniforms.VP = MathUtils::transpose(VP);
			uniforms.ITV = MathUtils::transpose(ITV);

			Material material{rc->get_material()};
			if (material.textures[TEX_DIFFUSE])
			{
				material.textures[TEX_DIFFUSE]->bind();
			}
			else
			{
				ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"dummyDiff.jpg")->bind();
			}

			D3D11_MAPPED_SUBRESOURCE ms;
			device_context->Map(m_skybox_uniform_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
			memcpy(ms.pData, &uniforms, sizeof(SkyboxUniformBuffer));
			device_context->Unmap(m_skybox_uniform_buffer.Get(), 0);

			device_context->VSSetConstantBuffers(0, 1, m_skybox_uniform_buffer.GetAddressOf());

			Mesh* mesh{rc->get_mesh()};

			//RenderStateManager::set(material.blend_state);

			mesh->get_vbo()->bind();

			if (mesh->get_index_count())
			{
				mesh->get_ibo()->bind();
				mesh->get_ibo()->draw();
			}
			else
			{
				mesh->get_vbo()->draw();
			}

			RenderStateManager::set(RenderStateType::DSS_DEPTH_MASK_1);
		}
}

void MainScene::setup_lights() noexcept
{
	Object* light1{new Object{"light1"}};

	LightComponent* lc1{new LightComponent{light1}};

	LightDesc light_desc;
	light_desc.ambient_intensity = Vec4f{0.0f, 0.0f, 0.0f, 0.0f};
	light_desc.diffuse_intensity = Vec4f{0.7f, 0.7f, 0.7f, 1.0f};
	light_desc.specular_intensity = Vec4f{1.0f, 1.0f, 1.0f, 1.0f};
	light_desc.flags = Vec4ui{1, 1, 0, 0};
	light_desc.attenuation = Vec3f{1.0f, 0.0f, 0.0f};
	light_desc.light_projection_matrix = MathUtils::perspective_lh(MathUtils::to_radians(80.0), 2048, 2048, 10.0f, 60.0f);
	lc1->set_light_description(light_desc);
	light1->set_position(Vec3f{0.0f, 30.0, 0.0f});
	light1->setup();
	add_object(light1);
	directional_desc = lc1->get_light_description_ptr();
	default_directional = light_desc;

	Object* light2{new Object{"light2"}};
	LightComponent* lc2{new LightComponent{light2}};
	LightDesc light_desc2;
	light_desc2.ambient_intensity = Vec4f{0.0f, 0.0f, 0.0f, 0.0f};
	light_desc2.diffuse_intensity = Vec4f{1.0f, 1.0f, 1.0f, 1.0f};
	light_desc2.specular_intensity = Vec4f{1.0f, 1.0f, 1.0f, 1.0f};
	light_desc2.flags = Vec4ui{0, 1, 0, 0};
	light_desc2.attenuation = Vec3f{1.0f, 0.0f, 0.0f};
	light_desc2.spot_cutoff = 20.0f;
	light_desc2.spot_exponent = 90.0f;
	light_desc2.spot_direction = Vec3f{0.09f, -0.1f, 0.09f};
	light_desc2.light_projection_matrix = MathUtils::perspective_lh(MathUtils::to_radians(60.0), 2048, 2048, 10.0f, 70.0f);
	lc2->set_light_description(light_desc2);
	light2->set_position(Vec3f{-30.0f, 30.0, -30.0f});
	light2->setup();
	add_object(light2);
	spotlight1_desc = lc2->get_light_description_ptr();
	default_spotlight1 = light_desc2;

	Object* light3{new Object{"light3"}};
	LightComponent* lc3{new LightComponent{light3}};
	LightDesc light_desc3;
	light_desc3.ambient_intensity = Vec4f{0.0f, 0.0f, 0.0f, 0.0f};
	light_desc3.diffuse_intensity = Vec4f{1.0f, 1.0f, 1.0f, 1.0f};
	light_desc3.specular_intensity = Vec4f{1.0f, 1.0f, 1.0f, 1.0f};
	light_desc3.flags = Vec4ui{0, 1, 0, 0};
	light_desc3.attenuation = Vec3f{1.0f, 0.0f, 0.0f};
	light_desc3.spot_cutoff = 20.0f;
	light_desc3.spot_exponent = 90.0f;
	light_desc3.spot_direction = Vec3f{-0.09f, -0.1f, 0.09f};
	light_desc3.light_projection_matrix = MathUtils::perspective_lh(MathUtils::to_radians(64.0), 2048, 2048, 10.0f, 60.0f);
	lc3->set_light_description(light_desc3);
	light3->set_position(Vec3f{30.0f, 30.0, -30.0f});
	light3->setup();
	add_object(light3);
	spotlight2_desc = lc3->get_light_description_ptr();
	default_spotlight2 = light_desc3;

	Object* light4{new Object{"light4"}};
	LightComponent* lc4{new LightComponent{light4}};
	LightDesc light_desc4;
	light_desc4.ambient_intensity = Vec4f{0.0f, 0.0f, 0.0f, 0.0f};
	light_desc4.diffuse_intensity = Vec4f{1.0f, 1.0f, 1.0f, 1.0f};
	light_desc4.specular_intensity = Vec4f{1.0f, 1.0f, 1.0f, 1.0f};
	light_desc4.flags = Vec4ui{0, 1, 0, 0};
	light_desc4.attenuation = Vec3f{1.0f, 0.0f, 0.0f};
	light_desc4.spot_cutoff = 20.0f;
	light_desc4.spot_exponent = 90.0f;
	light_desc4.spot_direction = Vec3f{0.0f, -0.1f, -0.1f};
	light_desc4.light_projection_matrix = MathUtils::perspective_lh(MathUtils::to_radians(64.0), 2048, 2048, 10.0f, 60.0f);
	lc4->set_light_description(light_desc4);
	light4->set_position(Vec3f{0.0f, 30.0, 30.0f});
	light4->setup();
	add_object(light4);
	spotlight3_desc = lc4->get_light_description_ptr();
	default_spotlight3 = light_desc4;
}

void MainScene::setup_cameras() noexcept
{
	float win_x = WindowingService::get_window(0)->get_size().x;
	float win_y = WindowingService::get_window(0)->get_size().y;

	Object* cam{new Object{"camera1"}};
	CameraComponent* cc{new CameraComponent{cam, MathUtils::to_radians(60.0f), win_x, win_y, 0.1f, 1000.0f}};
	CameraKeyboardInputComponent* input_comp{new CameraKeyboardInputComponent{cam}};
	input_comp->set_movement_speed(20.0f);
	input_comp->set_rotation_speed(90.0f);
	cam->set_position(Vec3f(0.0f, 0.0f, -42.0f));
	cam->setup();
	add_object(cam);

	Object* cam2{ new Object{ "camera2" } };
	CameraComponent* cc2{ new CameraComponent{ cam2, MathUtils::to_radians(60.0f), win_x, win_y, 0.1f, 1000.0f } };
	CameraKeyboardInputComponent* input_comp2 = new CameraKeyboardInputComponent{ cam2 };
	input_comp2->set_movement_speed(20.0f);
	input_comp2->set_rotation_speed(90.0f);
	cam2->set_position(Vec3f(11.0f, 11.0f, -11.0f));
	cam2->set_euler_angles(Vec3f{ 45.0f, -45.0f, 0.0f});
	cam2->setup();
	add_object(cam2);

	Object* cam3{new Object{"camera3"}};
	CameraComponent* cc3{new CameraComponent{cam3, MathUtils::to_radians(60.0f), win_x, win_y, 0.1f, 1000.0f}};
	input_comp = new CameraKeyboardInputComponent{cam3};
	input_comp->set_rotation_speed(90.0f);
	cam3->set_position(Vec3f(0.0f, 0.8f, -1.0f));
	cam3->set_euler_angles(Vec3f{ 30.0f, 0.0f, 0.0f });
	cam3->set_parent(m_followable_objects[0]);
	cam3->setup();
	add_object(cam3);
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
	D3D11Context* GAPI_context{EngineContext::get_GAPI_context()};
	ComPtr<ID3D11Device> device{GAPI_context->get_device()};

	HRESULT res{device->CreateBuffer(&cb_desc, nullptr, m_color_pass_uniform_buffer.ReleaseAndGetAddressOf())};

	if (FAILED(res))
	{
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

	if (FAILED(res))
	{
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

	if (FAILED(res))
	{
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

	if (FAILED(res))
	{
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

	if (FAILED(res))
	{
		std::cerr << "Active light structured buffer creation failed." << std::endl;
	}

	res = device->CreateShaderResourceView(m_light_structured_buffer.Get(), nullptr, m_light_srv.ReleaseAndGetAddressOf());

	if (FAILED(res))
	{
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

	if (FAILED(res))
	{
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

	if (FAILED(res))
	{
		std::cerr << "Linear Texture Wrap sampler creation failed!" << std::endl;
	}

	if (!TwInit(TW_DIRECT3D11, device.Get()))
	{
		std::cerr << "Failed to initialze AntTweakBar" << std::endl;
	}

	TwBar* tw_bar{TwNewBar("TweakBar")};

	Vec2i bar_size{260, 520};
	TwSetParam(tw_bar, nullptr, "size", TW_PARAM_INT32, 2, &bar_size[0]);

	TwAddButton(tw_bar, "toggle", toggle_directional_light, nullptr, "group=Directional");
	TwAddVarRW(tw_bar, "diffuse", TW_TYPE_COLOR3F, &directional_desc->diffuse_intensity.data, "group=Directional");
	TwAddVarRW(tw_bar, "specular", TW_TYPE_COLOR3F, &directional_desc->specular_intensity.data, "group=Directional");

	TwAddButton(tw_bar, "toggle spotlight1", toggle_spotlight_one, nullptr, "group=Spotlight1");
	TwAddVarRW(tw_bar, "spot1 diffuse", TW_TYPE_COLOR3F, &spotlight1_desc->diffuse_intensity.data, "group=Spotlight1");
	TwAddVarRW(tw_bar, "spot1 specular", TW_TYPE_COLOR3F, &spotlight1_desc->specular_intensity.data, "group=Spotlight1");
	TwAddVarRW(tw_bar, "spot1 cutoff", TW_TYPE_FLOAT, &spotlight1_desc->spot_cutoff, "group=Spotlight1 min=0 max=180");
	TwAddVarRW(tw_bar, "spot1 exponent", TW_TYPE_FLOAT, &spotlight1_desc->spot_exponent, "group=Spotlight1 min=0 max=128");
	TwAddVarRW(tw_bar, "spot1 direction", TW_TYPE_DIR3F, &spotlight1_desc->spot_direction[0], "group=Spotlight1");
	TwAddVarRW(tw_bar, "spot1 attenuation constant", TW_TYPE_FLOAT, &spotlight1_desc->attenuation[0], "group=Spotlight1 min=0 max=1 step=0.01");
	TwAddVarRW(tw_bar, "spot1 attenuation linear", TW_TYPE_FLOAT, &spotlight1_desc->attenuation[1], "group=Spotlight1 min=0 max=1 step=0.0001");
	TwAddVarRW(tw_bar, "spot1 attenuation quadratic", TW_TYPE_FLOAT, &spotlight1_desc->attenuation[2], "group=Spotlight1 min=0 max=1 step=0.00001");

	TwAddButton(tw_bar, "toggle spotlight2", toggle_spotlight_two, nullptr, "group=Spotlight2");
	TwAddVarRW(tw_bar, "spot2 diffuse", TW_TYPE_COLOR3F, &spotlight2_desc->diffuse_intensity.data, "group=Spotlight2");
	TwAddVarRW(tw_bar, "spot2 specular", TW_TYPE_COLOR3F, &spotlight2_desc->specular_intensity.data, "group=Spotlight2");
	TwAddVarRW(tw_bar, "spot2 cutoff", TW_TYPE_FLOAT, &spotlight2_desc->spot_cutoff, "group=Spotlight2 min=0 max=180");
	TwAddVarRW(tw_bar, "spot2 exponent", TW_TYPE_FLOAT, &spotlight2_desc->spot_exponent, "group=Spotlight2 min=0 max=128");
	TwAddVarRW(tw_bar, "spot2 direction", TW_TYPE_DIR3F, &spotlight2_desc->spot_direction[0], "group=Spotlight2");
	TwAddVarRW(tw_bar, "spot2 attenuation constant", TW_TYPE_FLOAT, &spotlight2_desc->attenuation[0], "group=Spotlight2 min=0 max=1 step=0.01");
	TwAddVarRW(tw_bar, "spot2 attenuation linear", TW_TYPE_FLOAT, &spotlight2_desc->attenuation[1], "group=Spotlight2 min=0 max=1 step=0.0001");
	TwAddVarRW(tw_bar, "spot2 attenuation quadratic", TW_TYPE_FLOAT, &spotlight2_desc->attenuation[2], "group=Spotlight2 min=0 max=1 step=0.00001");

	TwAddButton(tw_bar, "toggle spotlight3", toggle_spotlight_three, nullptr, "group=Spotlight3");
	TwAddVarRW(tw_bar, "spot3 diffuse", TW_TYPE_COLOR3F, &spotlight3_desc->diffuse_intensity.data, "group=Spotlight3");
	TwAddVarRW(tw_bar, "spot3 specular", TW_TYPE_COLOR3F, &spotlight3_desc->specular_intensity.data, "group=Spotlight3");
	TwAddVarRW(tw_bar, "spot3 cutoff", TW_TYPE_FLOAT, &spotlight3_desc->spot_cutoff, "group=Spotlight3 min=0 max=180");
	TwAddVarRW(tw_bar, "spot3 exponent", TW_TYPE_FLOAT, &spotlight3_desc->spot_exponent, "group=Spotlight3 min=0 max=128");
	TwAddVarRW(tw_bar, "spot3 direction", TW_TYPE_DIR3F, &spotlight3_desc->spot_direction[0], "group=Spotlight3");
	TwAddVarRW(tw_bar, "spot3 attenuation constant", TW_TYPE_FLOAT, &spotlight3_desc->attenuation[0], "group=Spotlight3 min=0 max=1 step=0.01");
	TwAddVarRW(tw_bar, "spot3 attenuation linear", TW_TYPE_FLOAT, &spotlight3_desc->attenuation[1], "group=Spotlight3 min=0 max=1 step=0.0001");
	TwAddVarRW(tw_bar, "spot3 attenuation quadratic", TW_TYPE_FLOAT, &spotlight3_desc->attenuation[2], "group=Spotlight3 min=0 max=1 step=0.00001");

	TwAddButton(tw_bar, "toggle shadows", toggle_shadows, nullptr, "group=Shadows");
	TwDefine("TweakBar/Directional group=Lights");
	TwDefine("TweakBar/Spotlight1 group=Lights");
	TwDefine("TweakBar/Spotlight2 group=Lights");
	TwDefine("TweakBar/Spotlight3 group=Lights");
	TwDefine("TweakBar/Shadows group=Lights");

	TwAddSeparator(tw_bar, "sep1", nullptr);

	TwAddButton(tw_bar, "Wireframe", toggle_wireframe, nullptr, nullptr);

	TwAddSeparator(tw_bar, "sep2", nullptr);

	TwAddButton(tw_bar, "Outer Camera", activate_outer_camera, nullptr, "key=f1");
	TwAddButton(tw_bar, "Inner Camera", activate_inner_camera, nullptr, "key=f2");
	TwAddButton(tw_bar, "Inner Follow Camera", activate_inner_follow_camera, nullptr, "key=f3");

	TwAddSeparator(tw_bar, "sep3", nullptr);

	TwAddVarRW(tw_bar, "Fresnel power", TW_TYPE_FLOAT, &fresnel_power, "min=1 max=10 step=0.1");
	TwAddVarRW(tw_bar, "Fresnel bias", TW_TYPE_FLOAT, &fresnel_bias, "min=0 max=1 step=0.01");

	TwAddSeparator(tw_bar, "sep4", nullptr);

	TwAddVarRW(tw_bar, "Simulation Speed", TW_TYPE_FLOAT, &simulation_speed, "min=0 max=10 step=0.1 keyincr=t keydecr=T");
	TwAddVarRO(tw_bar, "Milliseconds per frame", TW_TYPE_INT32, &dt_ms, nullptr);
	TwAddSeparator(tw_bar, "sep5", nullptr);

	TwAddButton(tw_bar, "Reset Scene", reset_scene, nullptr, "key=r");
}

void MainScene::cycle_camera_follower() noexcept
{
	static int idx{ 1 };
	CameraSystem* camera_system{ EngineContext::get_camera_system() };

	Object* cam{ camera_system->get_active_camera() };

	if( cam->get_name() != "camera3")
	{
		return;
	}

	camera_system->get_active_camera()->set_parent(m_followable_objects[idx]);

	idx++;

	if(idx > m_followable_objects.size() - 1)
	{
		idx = 0;
	}
}

void MainScene::initialize()
{
	Mesh* m{MeshUtils::generate_uv_sphere(1.0f, 60, 60)};
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
	mat.diffuse = Vec4f{0.1f, 0.1f, 0.1f, 1.0f};
	mat.specular = Vec4f{0.5f, 0.5f, 0.5f, 128.0f};
	mat.blend_state = RenderStateType::BS_BLEND_ALPHA;
	mat.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"sky.dds");

	m_globe = new Object{"globe"};
	RenderingComponent* rc{new RenderingComponent{m_globe}};
	rc->set_mesh(ResourceManager::get<Mesh>(L"sphere"));
	rc->set_material(mat);
	rc->set_casts_shadows(false);
	m_globe->set_position(Vec3f{0.0f, 0.0, 0.0f});
	m_globe->set_scale(Vec3f{20.0f, 20.0f, 20.0f});
	m_globe->calculate_xform();
	m_globe->setup();

	m_skybox = new Object{"skybox"};
	rc = new RenderingComponent{m_skybox};
	rc->set_mesh(ResourceManager::get<Mesh>(L"flipped_cube"));
	Material skybox_mat;
	skybox_mat.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"sky.dds");
	rc->set_material(skybox_mat);
	m_skybox->setup();

	Object* obj = new Object{"ground"};
	rc = new RenderingComponent{obj};
	rc->set_mesh(ResourceManager::get<Mesh>(L"cube"));
	mat.diffuse = Vec4f{1.0f, 1.0f, 1.0f, 1.0f};
	mat.specular = Vec4f{0.2f, 0.2f, 0.2f, 30.0f};
	mat.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"mars_diff.png");
	mat.textures[TEX_SPECULAR] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"mars_spec.png");
	mat.textures[TEX_NORMAL] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"mars_norm.png");
	mat.textures[TEX_DIFFUSE]->set_texture_type(TEX_DIFFUSE);
	mat.textures[TEX_SPECULAR]->set_texture_type(TEX_SPECULAR);
	mat.textures[TEX_NORMAL]->set_texture_type(TEX_NORMAL);
	mat.texture_matrix = MathUtils::scale(mat.texture_matrix, Vec3f{30.0f, 30.0f , 0.0f});
	rc->set_material(mat);
	rc->set_casts_shadows(false);
	obj->set_position(Vec3f{0.0f, -10.0, 0.0f});
	obj->set_scale(Vec3f{2000.0f, 0.1f, 2000.0f});
	obj->setup();
	add_object(obj);

	// Submarine 1 creation -------------------------------------------------------------------------------------------
	Object* drebel = new DrebelSubmarine{"drebel_sub", this};
	drebel->set_scale(Vec3f{6.0f, 6.0f, 6.0f});
	PathComponent* pc{new PathComponent{drebel}};
	pc->add_keyframe(Vec3f{10, 0, -10}, 0);
	pc->add_keyframe(Vec3f{ -10, -5, -10 }, 20000);
	pc->add_keyframe(Vec3f{ 10, 10, 10 }, 40000);
	pc->add_keyframe(Vec3f{ 10, 0, -10 }, 60000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	drebel->setup();
	m_followable_objects.push_back(drebel);

	Object* water_jet_sub = new WaterJetSubmarine{"water_jet_sub", this};
	water_jet_sub->set_scale(Vec3f{8.0f, 8.0f, 8.0f});
	pc = new PathComponent{ water_jet_sub };
	pc->add_keyframe(Vec3f{ -8, 0, 8 }, 0);
	pc->add_keyframe(Vec3f{ 8, -5, 8 }, 10000);
	pc->add_keyframe(Vec3f{ -8, 10, -8 }, 20000);
	pc->add_keyframe(Vec3f{ -8, 0, 8 }, 30000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	water_jet_sub->setup();
	m_followable_objects.push_back(water_jet_sub);

	float random_scale = MathUtils::random_range(1.0f, 5.0f);
	Object* deep_sea_fish1 = new DeepSeaFish{ "deep_sea_fish", this };
	deep_sea_fish1->set_scale(Vec3f{ random_scale, random_scale, random_scale });
	pc = new PathComponent{ deep_sea_fish1 };
	pc->add_keyframe(Vec3f{ -10, -5, 10 }, 0);
	pc->add_keyframe(Vec3f{ -0, -2, -10 }, 10000);
	pc->add_keyframe(Vec3f{ 10, -5, 10 }, 20000);
	pc->add_keyframe(Vec3f{ -10, -5, 10 }, 30000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	deep_sea_fish1->setup();
	m_followable_objects.push_back(deep_sea_fish1);

	random_scale = MathUtils::random_range(1.0f, 5.0f);
	Object* deep_sea_fish2 = new DeepSeaFish{ "deep_sea_fish2", this };
	deep_sea_fish2->set_scale(Vec3f{ random_scale, random_scale, random_scale });
	pc = new PathComponent{ deep_sea_fish2 };
	pc->add_keyframe(Vec3f{ 10, -5, -10 }, 0);
	pc->add_keyframe(Vec3f{ 0, -2, 10 }, 10000);
	pc->add_keyframe(Vec3f{ -10, -5, -10 }, 20000);
	pc->add_keyframe(Vec3f{ 10, -5, -10 }, 30000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	deep_sea_fish2->setup();
	m_followable_objects.push_back(deep_sea_fish2);

	random_scale = MathUtils::random_range(1.0f, 5.0f);
	Object* deep_sea_fish3 = new DeepSeaFish{ "deep_sea_fish3", this };
	deep_sea_fish3->set_scale(Vec3f{ random_scale, random_scale, random_scale });
	pc = new PathComponent{ deep_sea_fish3 };
	pc->add_keyframe(Vec3f{ -10, -1, 10 }, 0);
	pc->add_keyframe(Vec3f{ -0, 2, -10 }, 10000);
	pc->add_keyframe(Vec3f{ 10, -1, 10 }, 20000);
	pc->add_keyframe(Vec3f{ -10, -1, 10 }, 30000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	deep_sea_fish3->setup();
	m_followable_objects.push_back(deep_sea_fish3);

	random_scale = MathUtils::random_range(1.0f, 5.0f);
	Object* deep_sea_fish4 = new DeepSeaFish{ "deep_sea_fish4", this };
	deep_sea_fish4->set_scale(Vec3f{ random_scale, random_scale, random_scale });
	pc = new PathComponent{ deep_sea_fish4 };
	pc->add_keyframe(Vec3f{ 10, -1, -10 }, 0);
	pc->add_keyframe(Vec3f{ 0, 2, 10 }, 10000);
	pc->add_keyframe(Vec3f{ -10, -1, -10 }, 20000);
	pc->add_keyframe(Vec3f{ 10, -1, -10 }, 30000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	deep_sea_fish4->setup();
	m_followable_objects.push_back(deep_sea_fish4);

	random_scale = MathUtils::random_range(1.0f, 5.0f);
	Object* deep_sea_fish5 = new DeepSeaFish{ "deep_sea_fish5", this };
	deep_sea_fish5->set_scale(Vec3f{ random_scale, random_scale, random_scale });
	pc = new PathComponent{ deep_sea_fish5 };
	pc->add_keyframe(Vec3f{ -5, -1, 5 }, 0);
	pc->add_keyframe(Vec3f{ -0, 2, -5 }, 10000);
	pc->add_keyframe(Vec3f{ 5, -1, 5 }, 20000);
	pc->add_keyframe(Vec3f{ -5, -1, 5 }, 30000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	deep_sea_fish5->setup();
	m_followable_objects.push_back(deep_sea_fish5);

	random_scale = MathUtils::random_range(1.0f, 5.0f);
	Object* deep_sea_fish6 = new DeepSeaFish{ "deep_sea_fish6", this };
	deep_sea_fish6->set_scale(Vec3f{ random_scale, random_scale, random_scale });
	pc = new PathComponent{ deep_sea_fish6 };
	pc->add_keyframe(Vec3f{ 5, -1, -5 }, 0);
	pc->add_keyframe(Vec3f{ 0, 2, 5 }, 10000);
	pc->add_keyframe(Vec3f{ -5, -1, -5 }, 20000);
	pc->add_keyframe(Vec3f{ 5, -1, -5 }, 30000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	deep_sea_fish6->setup();
	m_followable_objects.push_back(deep_sea_fish6);

	random_scale = MathUtils::random_range(1.0f, 7.0f);
	Object* deep_sea_fish7 = new DeepSeaFish{ "deep_sea_fish7", this };
	deep_sea_fish7->set_scale(Vec3f{ random_scale, random_scale, random_scale });
	pc = new PathComponent{ deep_sea_fish7 };
	pc->add_keyframe(Vec3f{ -5, 4, 5 }, 0);
	pc->add_keyframe(Vec3f{ -0, 7, -5 }, 5000);
	pc->add_keyframe(Vec3f{ 5, 4, 5 }, 10000);
	pc->add_keyframe(Vec3f{ -5, 4, 5 }, 15000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	deep_sea_fish7->setup();
	m_followable_objects.push_back(deep_sea_fish7);

	random_scale = MathUtils::random_range(1.0f, 7.0f);
	Object* deep_sea_fish8 = new DeepSeaFish{ "deep_sea_fish8", this };
	deep_sea_fish8->set_scale(Vec3f{ random_scale, random_scale, random_scale });
	pc = new PathComponent{ deep_sea_fish8 };
	pc->add_keyframe(Vec3f{ 5, 4, -5 }, 0);
	pc->add_keyframe(Vec3f{ 0, 7, 5 }, 8000);
	pc->add_keyframe(Vec3f{ -5, 4, -5 }, 160000);
	pc->add_keyframe(Vec3f{ 5, 4, -5 }, 24000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	deep_sea_fish8->setup();
	m_followable_objects.push_back(deep_sea_fish8);

	random_scale = MathUtils::random_range(1.0f, 7.0f);
	Object* deep_sea_fish9 = new DeepSeaFish{ "deep_sea_fish9", this };
	deep_sea_fish9->set_scale(Vec3f{ random_scale, random_scale, random_scale });
	pc = new PathComponent{ deep_sea_fish9 };
	pc->add_keyframe(Vec3f{ -12, 4, 12 }, 0);
	pc->add_keyframe(Vec3f{ -0, 7, -12 }, 10000);
	pc->add_keyframe(Vec3f{ 12, 4, 12 }, 20000);
	pc->add_keyframe(Vec3f{ -12, 4, 12 }, 30000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	deep_sea_fish9->setup();
	m_followable_objects.push_back(deep_sea_fish9);

	random_scale = MathUtils::random_range(1.0f, 7.0f);
	Object* deep_sea_fish10 = new DeepSeaFish{ "deep_sea_fish10", this };
	deep_sea_fish10->set_scale(Vec3f{ random_scale, random_scale, random_scale });
	pc = new PathComponent{ deep_sea_fish10 };
	pc->add_keyframe(Vec3f{ 12, 4, -12 }, 0);
	pc->add_keyframe(Vec3f{ 0, 7, 12 }, 5000);
	pc->add_keyframe(Vec3f{ -12, 4, -12 }, 90000);
	pc->add_keyframe(Vec3f{ 12, 4, -12 }, 20000);
	pc->set_looping(true);
	pc->set_align_to_path(true);
	deep_sea_fish10->setup();
	m_followable_objects.push_back(deep_sea_fish10);
	// -----------------------------------------------------------------------------------------------------------------

	setup_cameras();

	setup_lights();

	Object* emitter{new Object{"emmiter1"}};
	EmitterComponent* ec{new EmitterComponent{emitter}};
	ec->set_lifespan(6.0);
	ec->set_max_particles(1000);
	ec->set_spawn_rate(20.0);
	ec->set_active(true);
	ec->set_particle_size(0.3f);
	ec->set_spawn_radius(13.0f);
	ec->set_velocity(Vec3f{0.0f, 0.0f, 0.0f});
	ec->set_velocity_range(0.05f);
	ec->set_external_force(Vec3f{0.0f, 0.1f, 0.0f});
	ec->set_mesh(ResourceManager::get<Mesh>(L"plane"));
	ec->set_start_color(Vec4f{1.0f, 1.0f, 1.0f, 1.0f});
	ec->set_end_color(Vec4f{1.0f, 1.0f, 1.0f, 0.3f});
	Material p_mat;
	p_mat.blend_state = RenderStateType::BS_BLEND_ADDITIVE;
	p_mat.textures[TEX_DIFFUSE] = ResourceManager::get<D3D11_texture>(TEXTURE_PATH + L"bubble10.png");
	p_mat.textures[TEX_DIFFUSE]->set_texture_type(TEX_DIFFUSE);
	ec->set_material(p_mat);
	emitter->setup();
	add_object(emitter);

	Window* win{WindowingService::get_window(0)};
	m_color_pass_rt.create(win->get_size());

	for (int i = 0; i < 4; ++i)
	{
		m_depth_pass_rts[i].create(Vec2f{2048, 2048});
	}

	setup_d3d();
}

void MainScene::on_key_down(unsigned char key, int x, int y) noexcept
{
	MessageContainer msg{new KeypressMessage{key, x, y, true}};
	Scene::on_message(msg);
}

void MainScene::on_key_up(unsigned char key, int x, int y) noexcept
{
	if (key == 115)
	{
		cycle_camera_follower();
	}

	MessageContainer msg{new KeypressMessage{key, x, y, false}};
	Scene::on_message(msg);
}

void MainScene::on_mouse_motion(int x, int y) noexcept
{
}

void MainScene::on_mouse_click(int button, bool state, int x, int y)
{
}


void MainScene::update(float delta_time, long time) noexcept
{
	dt_ms = static_cast<long>(delta_time * 1000.0f);
	Scene::update(delta_time * simulation_speed, time * simulation_speed);

	CameraSystem* camera_system{EngineContext::get_camera_system()};

	camera_system->process(get_objects(), delta_time * simulation_speed);

	LightSystem* light_system{EngineContext::get_light_system()};

	light_system->process(get_objects(), delta_time * simulation_speed);

	D3D11Context* GAPI_context{EngineContext::get_GAPI_context()};
	ComPtr<ID3D11DeviceContext> device_context{GAPI_context->get_device_context()};

	std::vector<LightDesc> lights{light_system->get_active_light_descriptions()};

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
