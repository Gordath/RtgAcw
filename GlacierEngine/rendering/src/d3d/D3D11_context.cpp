#include "D3D11_context.h"
#include <iostream>
#include <array>

namespace Glacier
{
	// Private Methods -----------------------------------------------------------------
	bool D3D11Context::create_D3D11_device_and_context()
	{
		unsigned int device_flags{ 0 };

/*#if defined(DEBUG) || defined(_DEBUG)
		device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
*/

		std::array<D3D_FEATURE_LEVEL, 3> feature_levels = {
			{ D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0 }
		};

		D3D_FEATURE_LEVEL feature_level;

		if (!m_device) {
			HRESULT h_result{ 0 };
			h_result = D3D11CreateDevice(nullptr,
			                             D3D_DRIVER_TYPE_HARDWARE,
			                             nullptr,
			                             device_flags,
			                             &feature_levels[0],
			                             feature_levels.size(),
			                             D3D11_SDK_VERSION,
			                             m_device.ReleaseAndGetAddressOf(),
			                             &feature_level,
			                             m_device_context.ReleaseAndGetAddressOf());

			if (FAILED(h_result)) {
				std::cerr << "D3D11CreateDevice Failed." << std::endl;
				return false;
			}

			if (feature_level != D3D_FEATURE_LEVEL_11_0) {
				std::cerr << "Direct3D Feature Level 11 unsupported." << std::endl;
				return false;
			}

/*#if defined(DEBUG) || defined(_DEBUG)
			h_result = m_device.As(&m_debug_interface);
			if (FAILED(h_result)) {
				std::cerr << "Failed to acquire ID3D11Debug interface!" << std::endl;
				return false;
			}
#endif*/
		}

		return true;
	}

	// ---------------------------------------------------------------------------------

	bool D3D11Context::create()
	{
		return create_D3D11_device_and_context();
	}

	ID3D11Device* D3D11Context::get_device() const
	{
		return m_device.Get();
	}

	ID3D11DeviceContext* D3D11Context::get_device_context() const
	{
		return m_device_context.Get();
	}

	unsigned int D3D11Context::get_MSAA_quality(int sample_count) const
	{
		unsigned int quality;
		m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
		                                       sample_count,
		                                       &quality);
		assert(quality > 0);

		return quality;
	}
}
