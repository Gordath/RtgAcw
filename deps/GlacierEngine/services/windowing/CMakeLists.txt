include_directories(include)
include_directories(../../include/internal)

set(SOURCE_FILES
      src/window.cpp
      src/win32window.cpp
	  src/d3d11window.cpp
      src/windowing_service.cpp
	  src/window_factory.cpp
	  include/window.h
	  include/win32window.h
	  include/d3d11window.h
	  include/windowing_service.h
	  include/window_factory.h)

add_library(windowing ${SOURCE_FILES})

target_link_libraries(windowing 
		d3d11.lib
		d3dx11.lib
		d3dx10.lib)
