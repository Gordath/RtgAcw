#ifndef GLACIER_ENGINE_D3D11WINDOW_H_
#define GLACIER_ENGINE_D3D11WINDOW_H_
#include "win32window.h"
#include <d3d11.h>
#include <D3D/d3dx11.h>
#include <D3D/d3dx10.h>

namespace Glacier
{
	class D3D11Window : public Win32Window
	{
	private:
		static ID3D11Device* _device;
		static ID3D11DeviceContext* _context;

		IDXGISwapChain* _swap_chain;

		bool _enable_MSAA = false;
		int _sample_count = 4;
		unsigned int _MSAA_quality;

		static bool create_D3D11_device(D3D11Window* window);
		static bool create_D3D11_swap_chain(D3D11Window* window);
		static unsigned int check_MSAA_quality(D3D11Window* window);
		static bool init_D3D(D3D11Window* window);

	public:
		D3D11Window(const std::string& title,
		            const Vec2i& size,
		            const Vec2i& position,
		            const unsigned int window_id,
		            const bool focused,
		            const bool minimized,
		            const bool resizeable,
		            const bool show_cursor,
		            const bool enable_MSAA,
		            const int MSAA_sample_count) : Win32Window(title,
		                                                       size,
		                                                       position,
		                                                       window_id,
		                                                       focused,
		                                                       minimized,
		                                                       resizeable,
		                                                       show_cursor),
		                                           _enable_MSAA(enable_MSAA),
		                                           _sample_count(MSAA_sample_count)
		{
			init_D3D(this);
		}

		void enable_MSAA(bool state);
		bool MSAA_enabled() const;

		int get_sample_count() const;
		unsigned int get_MSAA_quality() const;
	};
}

#endif //GLACIER_ENGINE_D3D11WINDOW_H_