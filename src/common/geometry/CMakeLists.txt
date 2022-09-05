
file(GLOB source_files **/*.cpp **/*.h *.cpp *.h)

add_library(common_geometry_lib STATIC ${source_files})

target_include_directories(common_geometry_lib PUBLIC ${INCLUDE_DIRS})

list(APPEND COMMON_LIBS common_geometry_lib)
set(COMMON_LIBS ${COMMON_LIBS} PARENT_SCOPE)
