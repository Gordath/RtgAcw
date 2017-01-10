#ifndef GLACIER_D3D11_TEXTURE_H_
#define GLACIER_D3D11_TEXTURE_H_
#include "texture.h"
#include "internal/types.h"
#include "D3D/D3D11.h"

namespace Glacier
{
	class D3D11_texture : public Texture {
	private:
		ComPtr<ID3D11ShaderResourceView> m_shader_resource_view;

	public:
		D3D11_texture(TextureType texture_type) : Texture{ texture_type }
		{
		}

		bool load(const std::wstring& file_name) noexcept override;

		void bind() const noexcept override;
	};
}

#endif
