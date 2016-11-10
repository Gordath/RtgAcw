#ifndef GLACIER_ENGINE_D3D11_TEXTURE_2D_H_
#define GLACIER_ENGINE_D3D11_TEXTURE_2D_H_

#include "texture_2d.h"

namespace Glacier
{
	class D3D11Texture2D : public Texture2D {
	public:
		bool create() override;

		void bind() override;
	};
}

#endif //GLACIER_ENGINE_D3D11_TEXTURE_2D_H_