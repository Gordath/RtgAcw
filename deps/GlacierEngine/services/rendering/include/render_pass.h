#ifndef GLACIER_ENGINE_RENDER_PASS_H_
#define GLACIER_ENGINE_RENDER_PASS_H_

#include "render_stage.h"
#include <vector>
#include <memory>

namespace Glacier
{
	class RenderPass {
	protected:
		std::vector<RenderStage*> _render_stages;

	public:
		virtual ~RenderPass() = default;

		void add_render_stage(RenderStage* render_stage);

		virtual void execute();
	};
}

#endif //GLACIER_ENGINE_RENDER_STAGE_H_