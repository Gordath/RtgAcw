#ifndef COLOR_PASS_H_
#define COLOR_PASS_H_
#include "../GlacierEngine/rendering/include/d3d/D3D11_render_target.h"
#include <vector>
#include "object.h"

class ColorPass {
private:
	

public:
	bool initialize() noexcept;

	void execute(const std::vector<Glacier::Object*>& objects) const noexcept;
};

#endif //COLOR_PASS_H_
