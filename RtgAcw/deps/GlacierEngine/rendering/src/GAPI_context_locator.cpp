#include "GAPI_context_locator.h"

namespace Glacier
{
	GAPIContext* GAPIContextLocator::_context;

	GAPIContext* GAPIContextLocator::get_GAPI_context()
	{
		return _context;
	}

	void GAPIContextLocator::provide(GAPIContext* context)
	{
		_context = context;
	}

}

