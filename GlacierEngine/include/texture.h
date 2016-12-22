#ifndef GLACIER_TEXTURE_H_
#define GLACIER_TEXTURE_H_
#include "resource.h"

namespace Glacier
{
	enum TextureType {
		TEX_DIFFUSE,
		TEX_SPECULAR,
		TEX_NORMAL,
		TEX_AMBIENT_OCCLUSION
	};

	class Texture : public Resource {
	protected:
		TextureType m_texture_type;

	public:
		Texture(TextureType texture_type) : m_texture_type{ texture_type }
		{
		}

		virtual void bind() const noexcept = 0;
	};
}

#endif //GLACIER_TEXTURE_H_
