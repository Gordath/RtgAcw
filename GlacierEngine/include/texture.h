#ifndef GLACIER_TEXTURE_H_
#define GLACIER_TEXTURE_H_
#include "resource.h"

namespace Glacier
{
	enum TextureType { /* parasoft-suppress  CODSTA-MCPP-03 "This enum is not scoped because it is used as a more readable and descriptive way to access specific texture array indices." */
		TEX_DIFFUSE,
		TEX_SPECULAR,
		TEX_NORMAL,
		TEX_EMISSION,
		TEX_AMBIENT_OCCLUSION,
		SUPPORTED_TEX_COUNT
	};

	class Texture : public Resource {
	private:
		TextureType m_texture_type;

	public:
		Texture() = default;

		explicit Texture(TextureType texture_type) : m_texture_type{ texture_type }
		{
		}

		virtual void bind() const noexcept = 0;

		void set_texture_type(TextureType texture_type) noexcept
		{
			m_texture_type = texture_type;
		}

		TextureType get_texture_type() const noexcept
		{
			return m_texture_type;
		}
	};
}

#endif //GLACIER_TEXTURE_H_
