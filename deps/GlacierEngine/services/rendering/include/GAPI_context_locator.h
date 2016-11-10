#ifndef GLACIER_ENGINE_GAPI_CONTEXT_LOCATOR_H_
#define GLACIER_ENGINE_GAPI_CONTEXT_LOCATOR_H_

#include "GAPI_context.h"

namespace Glacier
{
	class GAPIContextLocator {
	private:
		//Does not own the pointer.
		static GAPIContext* _context;
	protected:
		static GAPIContext* get_GAPI_context();

	public:
		static void provide(GAPIContext* context);
	};
}

#endif //GLACIER_ENGINE_GAPI_CONTEXT_LOCATOR_H_