#ifndef GLACIER_ENGINE_D3D11WINDOW_H_
#define GLACIER_ENGINE_D3D11WINDOW_H_
#include <D3D/d3d11.h>
#include "GAPI_context_locator.h"
#include "D3D11_render_target.h"
#include <memory>
#include "win32_window.h"

namespace Glacier
{
	class D3D11Context;

	class D3D11Window : public Win32Window, protected GAPIContextLocator {
	private:
		std::unique_ptr<D3D11RenderTarget> m_render_target;
		ComPtr<IDXGISwapChain> m_swap_chain;

		bool m_enable_MSAA = false;
		int m_sample_count = 4;
		unsigned int _MSAA_quality;

		bool create_D3D11_swap_chain(const D3D11Context* ctx);

		bool initialize();

	public:
		D3D11Window(const std::wstring& title,
		            const Vec2i& size,
		            const Vec2i& position,
		            const unsigned int window_id,
		            const bool focused,
		            const bool minimized,
		            const bool resizeable,
		            const bool show_cursor,
		            const bool enable_MSAA,
		            const int MSAA_sample_count,
		            const WindowFunctionCallbacks& callbacks) : Win32Window(title,
		                                                                    size,
		                                                                    position,
		                                                                    window_id,
		                                                                    focused,
		                                                                    minimized,
		                                                                    resizeable,
		                                                                    show_cursor,
		                                                                    callbacks),
		                                                        m_enable_MSAA(enable_MSAA),
		                                                        m_sample_count(MSAA_sample_count)
		{
			initialize();
		}

		void enable_MSAA(bool state);
		bool MSAA_enabled() const;

		int get_sample_count() const;
		unsigned int get_MSAA_quality() const;
	};
}

#endif //GLACIER_ENGINE_D3D11WINDOW_H_
