#ifndef GLACIER_RENDER_STATE_H_
#define GLACIER_RENDER_STATE_H_

namespace Glacier
{
	enum class RenderStateType {
		BLEND_DISSABLED,
		BLEND_ADDITIVE,
		BLEND_ALPHA
	};

	class RenderState {
	public:
		virtual ~RenderState() = default;

		virtual void set() const noexcept = 0;
	};
}

#endif //GLACIER_RENDER_STATE_H_