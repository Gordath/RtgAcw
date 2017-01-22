#ifndef GLACIER_ENGINE_D3D11_SHADER_H_
#define GLACIER_ENGINE_D3D11_SHADER_H_
#include "shader.h"
#include "internal/types.h"
#include <D3D/d3d11.h>

namespace Glacier
{
	class D3D11Shader : public Shader {
	private:
		ComPtr<ID3DBlob> m_shader_blob;

	public:
		D3D11Shader() = default;

		ID3DBlob* get_blob() const noexcept
		{
			return m_shader_blob.Get();
		}

		bool load(const std::wstring& file_name) noexcept override;
	};
}

#endif //GLACIER_ENGINE_D3D11_SHADER_H_
