#ifndef GLACIER_RENDER_STATE_H_
#define GLACIER_RENDER_STATE_H_

namespace Glacier
{
	enum class RenderStateType {
		BS_BLEND_DISSABLED,
		BS_BLEND_ADDITIVE,
		BS_BLEND_ALPHA,
		RS_CULL_FRONT,
		RS_CULL_BACK,
		RS_DRAW_WIRE,
		RS_DRAW_SOLID
	};

	class RenderState {
	public:
		virtual ~RenderState() = default;

		virtual void set() const noexcept = 0;
	};
}

#endif //GLACIER_RENDER_STATE_H_