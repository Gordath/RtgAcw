#ifndef SHADER_PROGRAM_MANAGER_H_
#define SHADER_PROGRAM_MANAGER_H_
#include <map>
#include <memory>
#include "../rendering/include/shader_program.h"

namespace Glacier
{
	using ShaderProgramMap = std::map<std::string, ShaderProgram*>;

	class ShaderProgramManager {
	private:
		static ShaderProgramMap shader_program_by_name;

	public:
		~ShaderProgramManager();

		static bool create(const std::string& prog_name,
		                   unsigned int input_layout_mask,
		                   const std::wstring& vs,
		                   const std::wstring& fs = L"",
		                   const std::wstring& hs = L"",
		                   const std::wstring& ds = L"",
		                   const std::wstring& gs = L"") noexcept;

		static ShaderProgram* get(const std::string& prog_name) noexcept;
	};
}

#endif //SHADER_PROGRAM_MANAGER_H_
