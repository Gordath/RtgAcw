set(SOURCE_FILES
	D3D11_render_target.cpp
	render_pass.cpp
	clear_render_stage.cpp
	GAPI_context_locator.cpp
	D3D11_context.cpp
	D3D11_texture_2d.cpp
	include/render_target.h
	include/D3D11_render_target.h
	include/render_pass.h
	include/render_stage.h
	include/clear_render_stage.h
	include/GAPI_context.h
	include/GAPI_context_locator.h
	include/D3D11_context.h
	include/texture_2d.h
	include/D3D11_texture_2d.h)

add_library(rendering ${SOURCE_FILES})