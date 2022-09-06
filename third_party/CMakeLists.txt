

add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(spdlog)
add_subdirectory(freetype)


list(APPEND COMMON_INCLUDE_DIRS ${GLFW_SOURCE_DIR}/include)
list(APPEND COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/glm)
list(APPEND COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)
list(APPEND COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/freetype/include)
list(APPEND COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

set(COMMON_INCLUDE_DIRS ${COMMON_INCLUDE_DIRS} PARENT_SCOPE)
