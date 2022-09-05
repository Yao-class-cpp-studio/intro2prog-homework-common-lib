
file(GLOB source_files **/*.cpp **/*.h *.cpp *.h)

add_library(common_opengl_lib STATIC ${source_files})

list(APPEND COMMON_LIBS common_opengl_lib)
set(COMMON_LIBS ${COMMON_LIBS} PARENT_SCOPE)

target_include_directories(common_opengl_lib PUBLIC ${INCLUDE_DIRS})
