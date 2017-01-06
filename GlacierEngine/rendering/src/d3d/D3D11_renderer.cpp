#include "D3D11_renderer.h"
#include "internal/engine_context.h"
#include "../../../../include/main_scene.h"
#include <iostream>
#include <minwinbase.h>
#include "light_component.h"

namespace Glacier
{
	struct UniformBuffer {
		Mat4f MVP;
		Mat4f MV;
		Mat4f M;
		Mat4f V;
		Mat4f P;
		Mat4f ITMV;
		Vec4f diffuse;
		Vec4f specular;
		Vec4f view_position;

		Vec4ui light_count;
	};

	bool D3D11Renderer::initialize() noexcept
	{
		UniformBuffer uniforms;

		D3D11_BUFFER_DESC cb_desc;
		ZeroMemory(&cb_desc, sizeof(cb_desc));
		cb_desc.ByteWidth = sizeof(uniforms);
		cb_desc.Usage = D3D11_USAGE_DYNAMIC;
		cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb_desc.MiscFlags = 0;
		cb_desc.StructureByteStride = 0;

		// Create the buffer.
		D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };
		ComPtr<ID3D11Device> device{ GAPI_context->get_device() };
		
		HRESULT res{ device->CreateBuffer(&cb_desc, nullptr, m_uniform_buffer.ReleaseAndGetAddressOf()) };

		if (FAILED(res)) {
			std::cerr << "Renderer initialization failed: Uniform buffer creation failed." << std::endl;
			return false;
		}

		//TODO: Create the light description structured buffer.
		D3D11_BUFFER_DESC sb_desc;
		ZeroMemory(&sb_desc, sizeof(sb_desc));
		sb_desc.ByteWidth = 10 * sizeof(LightDesc);
		sb_desc.Usage = D3D11_USAGE_DYNAMIC;
		sb_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		sb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		sb_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		sb_desc.StructureByteStride = sizeof(LightDesc);

		res = device->CreateBuffer(&sb_desc, nullptr, m_light_structured_buffer.ReleaseAndGetAddressOf());

		if (FAILED(res)) {
			std::cerr << "Active light structured buffer creation failed." << std::endl;
			return false;
		}

		res = device->CreateShaderResourceView(m_light_structured_buffer.Get(), nullptr, m_light_srv.ReleaseAndGetAddressOf());

		if (FAILED(res)) {
			std::cerr << "Light structured buffer shader resource view creation failed." << std::endl;
			return false;
		}

		return true;
	}

	void D3D11Renderer::draw(const std::vector<RenderingComponent*>& rendering_components, float delta_time)
	{
		D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };

		ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

		float cl_col[4]{ 0.2f, 0.2f, 0.2f, 0.0f };
		device_context->ClearRenderTargetView(GAPI_context->get_default_render_target_view(), cl_col);
		device_context->ClearDepthStencilView(GAPI_context->get_default_depth_stencil_view(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Renderer::draw(rendering_components, delta_time);
	}

	void D3D11Renderer::draw(Mesh* mesh, const Mat4f& model) noexcept
	{
		CameraSystem* camera_system{ EngineContext::get_camera_system() };

		//This is a temporary test.
		Mat4f view{ camera_system->get_active_camera_view_matrix() };

		Mat4f projection{ camera_system->get_active_camera_projection_matrix() };

		Mat4f MVP{ MathUtils::transpose(projection * view * model) };
		Mat4f MV{ MathUtils::transpose(view * model) };
		Mat4f ITMV{ MathUtils::transpose(MathUtils::inverse(MV)) };

		Vec4f diff{ 1.0f, 0.0f, 0.0f ,1.0f };
		Vec4f specular{ 1.0f, 1.0f, 1.0f, 1.0f };

		D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };
		ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

		LightSystem* light_system{ EngineContext::get_light_system() };

		auto lights{ light_system->get_active_light_descriptions() };

		D3D11_MAPPED_SUBRESOURCE lms;
		device_context->Map(m_light_structured_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &lms);
		memcpy(lms.pData, &lights[0], lights.size() * sizeof(LightDesc));
		device_context->Unmap(m_light_structured_buffer.Get(), 0);

		device_context->VSSetShaderResources(0, 1, m_light_srv.GetAddressOf());
		device_context->PSSetShaderResources(0, 1, m_light_srv.GetAddressOf());

		UniformBuffer uniforms;
		memcpy(&uniforms.MVP, &MVP[0][0], sizeof(Mat4f));
		memcpy(&uniforms.MV, &MV[0][0], sizeof(Mat4f));
		memcpy(&uniforms.M, &model[0][0], sizeof(Mat4f));
		memcpy(&uniforms.V, &view[0][0], sizeof(Mat4f));
		memcpy(&uniforms.P, &projection[0][0], sizeof(Mat4f));
		memcpy(&uniforms.ITMV, &ITMV[0][0], sizeof(Mat4f));
		memcpy(&uniforms.diffuse, &diff.data, sizeof(Vec4f));
		memcpy(&uniforms.specular, &specular.data, sizeof(Vec4f));
		uniforms.view_position = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };
		uniforms.light_count = Vec4i{lights.size(), 0, 0, 0};

		D3D11_MAPPED_SUBRESOURCE ms;

		device_context->Map(m_uniform_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		memcpy(ms.pData, &uniforms, sizeof(UniformBuffer));
		device_context->Unmap(m_uniform_buffer.Get(), 0);

		device_context->VSSetConstantBuffers(0, 1, m_uniform_buffer.GetAddressOf());
		device_context->PSSetConstantBuffers(0, 1, m_uniform_buffer.GetAddressOf());

		device_context->OMSetRenderTargets(1, GAPI_context->get_address_of_render_target_view(), GAPI_context->get_default_depth_stencil_view());

		mesh->get_vbo()->bind();

		if (mesh->get_index_count()) {
			mesh->get_ibo()->bind();
			mesh->get_ibo()->draw();
		} else {
			mesh->get_vbo()->draw();
		}
	}
}
