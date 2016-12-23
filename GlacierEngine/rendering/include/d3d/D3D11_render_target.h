#ifndef GLACIER_ENGINE_D3D_RENDER_TARGET_H_
#define GLACIER_ENGINE_D3D_RENDER_TARGET_H_
#include "render_target.h"
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

		ComPtr<ID3D11ShaderResourceView> m_color_attachment_srv; //TODO: initialize this.
		ComPtr<ID3D11ShaderResourceView> m_depth_attachment_srv; //TODO: initialize this.

		bool m_MSAA = false;
		int m_sample_count = 1;

	public:
		D3D11RenderTarget(const Vec2i& size,
		                  bool MSAA,
		                  int sample_count,
		                  ID3D11Texture2D* back_buffer = nullptr) : RenderTarget(size),
		                                                            m_MSAA(MSAA),
		                                                            m_sample_count(sample_count)
		{
			m_color_attachment.Attach(back_buffer);
		}

		bool create() override;

		bool bind() const override;

		bool unbind() const override;
	};
}

#endif //GLACIER_ENGINE_D3D_RENDER_TARGET_H_
