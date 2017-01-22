#ifndef GLACIER_ENGINE_RENDER_TARGET_H_
#define GLACIER_ENGINE_RENDER_TARGET_H_
#include "internal/types.h"

namespace Glacier
{
	enum class RenderTargetBindType {
		COLOR_AND_DEPTH,
		DEPTH
	};

	class RenderTarget {
	protected:
		Vec2i m_size;

	public:
		RenderTarget() = default;

		explicit RenderTarget(const Vec2i& size) : m_size{ size }
		{
		}

		virtual ~RenderTarget() = default;

		virtual bool create(const Vec2i& size) = 0;

		virtual bool bind(RenderTargetBindType bind_type) const = 0;

		virtual bool unbind() const = 0;

		const Vec2i& get_size() const noexcept
		{
			return m_size;
		}
	};
}

#endif //GLACIER_ENGINE_RENDER_TARGET_H_
