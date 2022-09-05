

add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(spdlog)
add_subdirectory(freetype)


list(APPEND INCLUDE_DIRS ${GLFW_SOURCE_DIR}/include)
list(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/glm)
list(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)
list(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/freetype/include)
list(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
