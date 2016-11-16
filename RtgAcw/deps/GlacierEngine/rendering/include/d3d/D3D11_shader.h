#ifndef GLACIER_ENGINE_D3D11_SHADER_H_
#define GLACIER_ENGINE_D3D11_SHADER_H_
#include "shader.h"
#include "types.h"
#include <D3D/d3d11.h>

namespace Glacier
{
	class D3D11Shader : public Shader {
	private:
		ComPtr<ID3DBlob> _shader_blob;

		bool compile(const std::string &sdr) override;
	public:

	};
}

#endif //GLACIER_ENGINE_D3D11_SHADER_H_