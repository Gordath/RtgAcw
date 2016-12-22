#ifndef GLACIER_RENDERING_COMPONENT_H_
#define GLACIER_RENDERING_COMPONENT_H_
#include "component.h"
#include "mesh.h"

namespace Glacier
{
	class RenderingComponent : public Component {
	private:
		Mesh* m_mesh{ nullptr };
		
		//MAterial

		bool m_should_draw{ true };

	public:
		RenderingComponent(Object* parent) : Component{ "co_rendering", parent }
		{	
		}

		void setup() noexcept override;

		void update(float dt, long time) noexcept override;

		void teardown() noexcept override;
	};
}

#endif //GLACIER_RENDERING_COMPONENT_H_
