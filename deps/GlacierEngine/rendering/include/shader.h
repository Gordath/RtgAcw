#ifndef GLACIER_ENGINE_SHADER_H_
#define GLACIER_ENGINE_SHADER_H_
#include <string>
#include "GAPI_context_locator.h"

namespace Glacier
{
	class Shader : protected GAPIContextLocator {
	protected:
		virtual bool compile(const std::string &sdr) = 0;

	public:
		virtual ~Shader() = default;

		virtual bool load(const std::string &sdr) = 0;
	};
}

#endif //GLACIER_ENGINE_SHADER_H_