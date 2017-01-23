#include "render_state_manager.h"
#include "D3D11_blend_state.h"
#include "D3D11_rasterizer_state.h"
#include "D3D11_depth_stencil_state.h"

namespace Glacier
{
	std::map<RenderStateType, RenderState*> RenderStateManager::m_render_states;

	RenderStateManager::~RenderStateManager()
	{
		try {
			for (auto render_state : m_render_states) {
				delete render_state.second;
			}
		}
		catch(...){
		}

		m_render_states.clear();
	}


	RenderStateManager::RenderStateManager()
	{
	}

	void RenderStateManager::initialize() noexcept
	{
#if defined(GLACIERENGINE_BUILD_D3D)
		m_render_states[RenderStateType::BS_BLEND_DISSABLED] = new D3D11BlendState{ RenderStateType::BS_BLEND_DISSABLED };
		m_render_states[RenderStateType::BS_BLEND_ADDITIVE] = new D3D11BlendState{ RenderStateType::BS_BLEND_ADDITIVE };
		m_render_states[RenderStateType::BS_BLEND_ALPHA] = new D3D11BlendState{ RenderStateType::BS_BLEND_ALPHA };
		m_render_states[RenderStateType::RS_CULL_FRONT] = new D3D11RasterizerState{ RenderStateType::RS_CULL_FRONT };
		m_render_states[RenderStateType::RS_CULL_BACK] = new D3D11RasterizerState{ RenderStateType::RS_CULL_BACK };
		m_render_states[RenderStateType::RS_DRAW_SOLID] = new D3D11RasterizerState{ RenderStateType::RS_DRAW_SOLID };
		m_render_states[RenderStateType::RS_DRAW_WIRE] = new D3D11RasterizerState{ RenderStateType::RS_DRAW_WIRE };
		m_render_states[RenderStateType::DSS_DEPTH_TEST_ENABLE] = new D3D11DepthStencilState{ RenderStateType::DSS_DEPTH_TEST_ENABLE };
		m_render_states[RenderStateType::DSS_DEPTH_TEST_DISABLE] = new D3D11DepthStencilState{ RenderStateType::DSS_DEPTH_TEST_DISABLE };
		m_render_states[RenderStateType::DSS_DEPTH_MASK_0] = new D3D11DepthStencilState{ RenderStateType::DSS_DEPTH_MASK_0 };
		m_render_states[RenderStateType::DSS_DEPTH_MASK_1] = new D3D11DepthStencilState{ RenderStateType::DSS_DEPTH_MASK_1 };
#else
#endif
	}

	void RenderStateManager::set(RenderStateType render_state) noexcept
	{
		m_render_states[render_state]->set();
	}
}
