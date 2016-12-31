#include "D3D11_renderer.h"
#include "internal/engine_context.h"
#include "../../../../include/main_scene.h"
#include <iostream>

namespace Glacier
{
	struct UniformBuffer {
		Mat4f MVP;
		Mat4f MV;
		Mat4f ITMV;
		Vec4f diffuse;
		Vec4f specular;
		Vec4f view_position;
	};

	bool D3D11Renderer::initialize() noexcept
	{
		UniformBuffer uniforms;

		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(uniforms);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &uniforms;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		// Create the buffer.
		D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };
		ComPtr<ID3D11Device> device{ GAPI_context->get_device() };
		
		HRESULT res{ device->CreateBuffer(&cbDesc, &InitData, m_uniform_buffer.ReleaseAndGetAddressOf()) };

		if (FAILED(res)) {
			std::cerr << "Renderer initialization failed: Uniform buffer creation failed." << std::endl;
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

	void D3D11Renderer::draw(Mesh* mesh) noexcept
	{
		//This is a temporary test.
		Mat4f view;
		view = MathUtils::translate(view, Vec3f{ 0.0f, 0.0f, -2.0f });
		view = MathUtils::inverse(view);

		Mat4f model;
		//model = MathUtils::translate(model, Vec3f{ 0.0f, 0.0f, 5.0f });

		Mat4f proj;
		proj = MathUtils::perspective_lh(proj, MathUtils::to_radians(60.0f), 1024, 768, 0.1f, 1000.0f);

		Mat4f MVP{ MathUtils::transpose(proj * view * model) };
		Mat4f MV{ MathUtils::transpose(view * model) };
		Mat4f ITMV{ MathUtils::transpose(MathUtils::inverse(MV)) };

		Vec4f diff{ 1.0f, 0.0f, 0.0f ,1.0f };
		Vec4f specular{ 1.0f, 1.0f, 1.0f, 1.0f };

		UniformBuffer uniforms;
		memcpy(&uniforms.MVP, &MVP[0][0], sizeof(Mat4f));
		memcpy(&uniforms.MV, &MV[0][0], sizeof(Mat4f));
		memcpy(&uniforms.ITMV, &ITMV[0][0], sizeof(Mat4f));
		memcpy(&uniforms.diffuse, &diff.data, sizeof(Vec4f));
		memcpy(&uniforms.specular, &specular.data, sizeof(Vec4f));
		uniforms.view_position = Vec4f{ 0.0f, 0.0f, 0.0f, 0.0f };

		D3D11Context* GAPI_context{ EngineContext::get_GAPI_context() };
		ComPtr<ID3D11DeviceContext> device_context{ GAPI_context->get_device_context() };

		D3D11_MAPPED_SUBRESOURCE ms;

		device_context->Map(m_uniform_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms); // map the buffer
		memcpy(ms.pData, &uniforms, sizeof(UniformBuffer)); // copy the data
		device_context->Unmap(m_uniform_buffer.Get(), 0); // unmap the buffer

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
