#include "clear_render_stage.h"

namespace Glacier
{
	void ClearRenderStage::execute()
	{
#if defined(GLACIERENGINE_BUILD_D3D)
		//TODO: clear D3D
#else
		//TODO: clear GL
#endif
	}
}

