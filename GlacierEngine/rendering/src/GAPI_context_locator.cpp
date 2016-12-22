#include "GAPI_context_locator.h"

namespace Glacier
{
	GAPIContext* GAPIContextLocator::m_context;

	GAPIContext* GAPIContextLocator::get_GAPI_context()
	{
		return m_context;
	}

	void GAPIContextLocator::provide(GAPIContext* context)
	{
		m_context = context;
	}
}
