#include "shader_program_manager.h"
#include <iostream>
#include "D3D11_shader_program.h"

namespace Glacier
{
	ShaderProgramMap ShaderProgramManager::shader_program_by_name;

	bool ShaderProgramManager::create(const std::string& prog_name,
	                                  unsigned input_layout_mask,
	                                  const std::wstring& vs,
	                                  const std::wstring& fs,
	                                  const std::wstring& hs,
	                                  const std::wstring& ds,
	                                  const std::wstring& gs) noexcept
	{
		ShaderProgram* prog_check{ shader_program_by_name[prog_name].get() };

		if (prog_check) {
			std::cerr << "A shader program with the name provided already exists. Skipping creation." << std::endl;
			return false;
		}

		std::shared_ptr<ShaderProgram> sdr_prog;

#if defined(GLACIERENGINE_BUILD_D3D)
		sdr_prog = std::make_shared<D3D11ShaderProgram>();
#endif

		if (!sdr_prog->create(input_layout_mask,
		                      vs,
		                      fs,
		                      hs,
		                      ds,
		                      gs)) {
			return false;
		}

		shader_program_by_name[prog_name] = sdr_prog;

		return true;
	}

	std::shared_ptr<ShaderProgram> ShaderProgramManager::get(const std::string& prog_name) noexcept
	{
		return shader_program_by_name[prog_name];
	}
}
