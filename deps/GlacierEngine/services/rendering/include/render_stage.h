#ifndef GLACIER_ENGINE_RENDER_STAGE_H_
#define GLACIER_ENGINE_RENDER_STAGE_H_

namespace Glacier
{
	class RenderStage {
	public:
		virtual ~RenderStage() = default;

		virtual void execute() = 0;
	};
}

#endif //GLACIER_ENGINE_RENDER_STAGE_H_
