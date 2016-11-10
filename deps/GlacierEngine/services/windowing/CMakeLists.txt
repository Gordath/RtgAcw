include_directories(include)
include_directories(../../include/internal)

set(SOURCE_FILES
      src/window.cpp
      src/win32_window.cpp
	  src/D3D11_window.cpp
      src/windowing_service.cpp
	  src/window_factory.cpp
	  include/window.h
	  include/win32_window.h
	  include/D3D11_window.h
	  include/windowing_service.h
	  include/window_factory.h
	  include/win32_utils.h)

add_library(windowing ${SOURCE_FILES})

target_link_libraries(windowing 
		d3d11.lib
		d3dx11.lib
		d3dx10.lib
		)
