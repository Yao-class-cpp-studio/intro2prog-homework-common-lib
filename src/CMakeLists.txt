
list(APPEND COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(COMMON_INCLUDE_DIRS ${COMMON_INCLUDE_DIRS} PARENT_SCOPE)

add_subdirectory(common/geometry)
add_subdirectory(common/opengl)

add_library(common_lib STATIC common/common.cpp common/common.h)
target_link_libraries(common_lib PRIVATE ${COMMON_LIBS})
