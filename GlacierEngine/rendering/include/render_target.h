#ifndef GLACIER_ENGINE_RENDER_TARGET_H_
#define GLACIER_ENGINE_RENDER_TARGET_H_
#include "internal/types.h"

namespace Glacier
{
	class RenderTarget {
	protected:
		Vec2i m_size;

	public:
		RenderTarget() = default;

		RenderTarget(const Vec2i& size) : m_size{ size }
		{
		}

		virtual ~RenderTarget() = default;

		virtual bool create(const Vec2i& size) = 0;

		virtual bool bind() const = 0;

		virtual bool unbind() const = 0;
	};
}

#endif //GLACIER_ENGINE_RENDER_TARGET_H_
