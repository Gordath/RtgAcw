#ifndef GLACIER_ENGINE_CLEAR_RENDER_STAGE_H_
#define GLACIER_ENGINE_CLEAR_RENDER_STAGE_H_

#include "render_stage.h"
#include "types.h"

namespace Glacier
{
	class ClearRenderStage : public RenderStage {
	private:
		Vec4f _color;

	public:
		ClearRenderStage(const Vec4f &color) : _color(color)
		{ }

		void execute() override;
	};
}

#endif //GLACIER_ENGINE_CLEAR_RENDER_STAGE_H_