#ifndef GLACIER_D3D11_BLEND_STATE_H_
#define GLACIER_D3D11_BLEND_STATE_H_
#include "render_state.h"
#include "internal/types.h"
#include "D3D/d3d11.h"

namespace Glacier
{
	class D3D11BlendState : public RenderState {
	private:
		ComPtr<ID3D11BlendState> m_blend_state;

	public:
		explicit D3D11BlendState(RenderStateType render_state_type);

		void set() const noexcept override;
	};
}

#endif //GLACIER_D3D11_BLEND_STATE_H_
