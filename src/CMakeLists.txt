
list(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

add_subdirectory(common/geometry)
add_subdirectory(common/opengl)

set(COMMON_LIBS ${COMMON_LIBS} PARENT_SCOPE)
