#ifndef GLACIER_D3D11_DEPTH_STENCIL_STATE_H_
#define GLACIER_D3D11_DEPTH_STENCIL_STATE_H_
#include "render_state.h"
#include "internal/types.h"
#include "D3D/D3D11.h"


namespace Glacier
{
	class D3D11DepthStencilState : public RenderState {
	private:
		ComPtr<ID3D11DepthStencilState> m_depth_stencil_state;

	public:
		D3D11DepthStencilState(RenderStateType render_state_type);

		void set() const noexcept override;
	};
}

#endif //GLACIER_D3D11_DEPTH_STENCIL_STATE_H_
