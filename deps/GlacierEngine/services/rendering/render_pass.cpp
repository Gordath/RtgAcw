#include "render_pass.h"

namespace Glacier
{
	void RenderPass::add_render_stage(RenderStage* render_stage)
	{
		_render_stages.push_back(render_stage);
	}
}
