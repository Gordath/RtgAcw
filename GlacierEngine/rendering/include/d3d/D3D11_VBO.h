#ifndef GLACIER_ENGINE_D3D11_VBO_H_
#define GLACIER_ENGINE_D3D11_VBO_H_
#include "VBO.h"
#include <D3D/d3d11.h>
#include "internal/types.h"

namespace Glacier
{
	class D3D11VBO : public VBO {
	private:
		ComPtr<ID3D11Buffer> m_vertex_buffer;

	public:
		bool create(const std::vector<Vertex>& vertices, PrimitiveTopology primitive_topology) noexcept override;

		void bind() const noexcept override;

		void draw() const noexcept override;
	};
}

#endif //GLACIER_ENGINE_D3D11_VBO_H_
