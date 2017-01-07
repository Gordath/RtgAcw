#include "render_state_manager.h"
#include "D3D11_blend_state.h"

namespace Glacier
{
	std::map<RenderStateType, RenderState*> RenderStateManager::m_render_states;

	RenderStateManager::~RenderStateManager()
	{
		for (auto render_state : m_render_states) {
			delete render_state.second;
		}

		m_render_states.clear();
	}


	void RenderStateManager::initialize() noexcept
	{
#if defined(GLACIERENGINE_BUILD_D3D)
		m_render_states[RenderStateType::BLEND_DISSABLED] = new D3D11BlendState(RenderStateType::BLEND_DISSABLED);
		m_render_states[RenderStateType::BLEND_ADDITIVE] = new D3D11BlendState(RenderStateType::BLEND_ADDITIVE);
		m_render_states[RenderStateType::BLEND_ALPHA] = new D3D11BlendState(RenderStateType::BLEND_ALPHA);
#else
#endif
	}

	void RenderStateManager::set(RenderStateType render_state) noexcept
	{
		m_render_states[render_state]->set();
	}
}
