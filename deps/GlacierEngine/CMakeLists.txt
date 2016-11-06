add_definitions("-DGLM_FORCE_RADIANS")
add_definitions("-DGLM_PRECISION_HIGHP_FLOAT")
add_definitions("-DGLM_PRECISION_HIGHP_INT")
add_definitions("-DGLM_FORCE_SWIZZLE")

add_definitions(-DGLACIERENGINE_STATIC)
add_definitions(-DGLACIERENGINE_EXPORTS)

include_directories(include/internal)
include_directories(include)
include_directories(services/windowing/include)
include_directories(C:/usr/include)

add_subdirectory(services)

set(SOURCE_FILES
        src/application.cpp
        src/service_provider.cpp
		include/glacier_engine.h
		include/internal/dll_export.h
		include/internal/application.h
		include/internal/service.h
		include/internal/service_provider.h
		include/internal/types.h)

add_library(GlacierEngine ${SOURCE_FILES})

target_link_libraries(GlacierEngine windowing)

