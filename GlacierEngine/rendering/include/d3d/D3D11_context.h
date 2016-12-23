#ifndef GLACIER_ENGINE_D3D_CONTEXT_H_
#define GLACIER_ENGINE_D3D_CONTEXT_H_

#include "../GAPI_context.h"
#include <D3D/d3d11.h>
#include "../../../include/internal/types.h"

namespace Glacier
{
	class D3D11Context : public GAPIContext {
	private:
		bool create_D3D11_device_and_context();

	protected:
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_device_context;
		ComPtr<ID3D11Debug> m_debug_interface;

	public:
		~D3D11Context()
		{
			m_device.Reset();
			m_device_context.Reset();
			m_debug_interface.Reset();
		}

		bool create() override;

		ID3D11Device* get_device() const;
		ID3D11DeviceContext* get_device_context() const;
		ID3D11Debug* get_debug_interface() const { return m_debug_interface.Get(); }

		unsigned int get_MSAA_quality(int sample_count) const;
	};
}

#endif //GLACIER_ENGINE_D3D_CONTEXT_H_
