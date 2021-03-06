#ifndef GLACIER_ENGINE_D3D11_IBO_H_
#define GLACIER_ENGINE_D3D11_IBO_H_
#include "IBO.h"
#include "internal/types.h"
#include <D3D/d3d11.h>

namespace Glacier
{
	class D3D11IBO : public IBO {
	private:
		ComPtr<ID3D11Buffer> m_index_buffer;

	public:
		D3D11IBO() = default;

		bool create(const std::vector<unsigned int>& indices) noexcept override;

		void bind() const noexcept override;

		void draw() const noexcept override;
	};
}

#endif //GLACIER_ENGINE_D3D11_IBO_H_
