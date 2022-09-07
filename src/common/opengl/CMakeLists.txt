
file(GLOB source_files **/*.cpp **/*.h *.cpp *.h)

add_library(common_opengl_lib STATIC ${source_files})

target_include_directories(common_opengl_lib PRIVATE ${COMMON_INCLUDE_DIRS})

target_link_libraries(common_opengl_lib PRIVATE glad glfw freetype common_geometry_lib)

list(APPEND COMMON_LIBS common_opengl_lib)
set(COMMON_LIBS ${COMMON_LIBS} PARENT_SCOPE)
