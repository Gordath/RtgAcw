cmake_minimum_required(VERSION 3.6)
project(RtgAcw)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(deps)

include_directories(include)
include_directories(deps/GlacierEngine/include)
include_directories(deps/GlacierEngine/include/internal)
include_directories(deps/GlacierEngine/windowing/include)
include_directories(deps/GlacierEngine/windowing/include/d3d)
include_directories(deps/GlacierEngine/windowing/include/win32)
include_directories(deps/GlacierEngine/rendering/include)
include_directories(deps/GlacierEngine/rendering/include/d3d)
include_directories(C:/usr/include)

link_directories(C:/usr/lib)

set(SOURCE_FILES
	include/rtg_application.h
	src/main.cpp
	src/rtg_application.cpp)

add_executable(RtgAcw ${SOURCE_FILES})
target_link_libraries(RtgAcw GlacierEngine)