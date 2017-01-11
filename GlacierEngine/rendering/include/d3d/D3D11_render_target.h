#ifndef GLACIER_ENGINE_D3D_RENDER_TARGET_H_
#define GLACIER_ENGINE_D3D_RENDER_TARGET_H_
#include "../render_target.h"
#include "internal/types.h"
#include <D3D/d3d11.h>

namespace Glacier
{
	class D3D11RenderTarget : public RenderTarget {
	private:
		ComPtr<ID3D11RenderTargetView> m_render_target_view;
		ComPtr<ID3D11DepthStencilView> m_depth_buffer_view;

		ComPtr<ID3D11Texture2D> m_color_attachment;
		ComPtr<ID3D11Texture2D> m_depth_attachment;

		ComPtr<ID3D11ShaderResourceView> m_color_attachment_srv;
		ComPtr<ID3D11ShaderResourceView> m_depth_attachment_srv;

		bool m_MSAA = false;
		int m_sample_count = 1;

	public:
		D3D11RenderTarget() = default;

		D3D11RenderTarget(const Vec2i& size,
		                  bool MSAA,
		                  int sample_count) : RenderTarget(size),
		                                      m_MSAA(MSAA),
		                                      m_sample_count(sample_count)
		{
		}

		bool create(const Vec2i& size) override;

		bool bind(RenderTargetBindType bind_type) const override;

		bool unbind() const override;

		void clear(float* color) const noexcept;

		ID3D11ShaderResourceView* get_color_attachment() const noexcept
		{
			return m_color_attachment_srv.Get();
		}

		ID3D11ShaderResourceView* get_depth_attachment() const noexcept
		{
			return m_depth_attachment_srv.Get();
		}
	};
}

#endif //GLACIER_ENGINE_D3D_RENDER_TARGET_H_
