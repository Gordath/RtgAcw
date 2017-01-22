#ifndef GLACIER_D3D11_RASTERIZER_STATE_H_
#define GLACIER_D3D11_RASTERIZER_STATE_H_
#include "render_state.h"
#include "internal/types.h"
#include "D3D11_IBO.h"

namespace Glacier
{
	class D3D11RasterizerState : public RenderState {
	private:
		ComPtr<ID3D11RasterizerState> m_rasterizer_state;

	public:
		explicit D3D11RasterizerState(RenderStateType render_state_type);

		void set() const noexcept override;
	};
}

#endif //GLACIER_D3D11_RASTERIZER_STATE_H_
