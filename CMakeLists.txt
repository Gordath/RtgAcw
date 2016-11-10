cmake_minimum_required(VERSION 3.6)
project(RtgAcw)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(deps)

include_directories(deps/GlacierEngine/include)
include_directories(deps/GlacierEngine/include/internal)
include_directories(deps/GlacierEngine/services/windowing/include)
include_directories(deps/GlacierEngine/services/rendering/include)
include_directories(C:/usr/include)
link_directories(C:/usr/lib)

set(SOURCE_FILES
    src/util/types.h
    src/main.cpp
	src/rtg_application.cpp
	src/rtg_application.h)

add_executable(RtgAcw ${SOURCE_FILES})
target_link_libraries(RtgAcw GlacierEngine)