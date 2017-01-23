#ifndef GLACIER_RENDER_STATE_MANAGER_H_
#define GLACIER_RENDER_STATE_MANAGER_H_
#include <map>
#include "render_state.h"

namespace Glacier
{
	class RenderStateManager {
	private:
		static std::map<RenderStateType, RenderState*> m_render_states;

	public:
		RenderStateManager();

		~RenderStateManager();

		RenderStateManager(const RenderStateManager&) = delete;

		RenderStateManager& operator=(const RenderStateManager&) = delete;

		static void initialize() noexcept;

		static void set(RenderStateType render_state) noexcept;
	};
}

#endif //GLACIER_RENDER_STATE_MANAGER_H_
