#ifndef GLACIER_ENGINE_GAPI_CONTEXT_H_
#define GLACIER_ENGINE_GAPI_CONTEXT_H_

namespace Glacier
{
	class GAPIContext {
	public:
		virtual ~GAPIContext() = default;
		virtual bool create() = 0;
	};
}

#endif //GLACIER_ENGINE_GAPI_CONTEXT_H_