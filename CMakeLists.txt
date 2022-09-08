 cmake_minimum_required(VERSION 3.22)

project(homework-common)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(third_party)
add_subdirectory(src)

if (NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    set(COMMON_INCLUDE_DIRS ${COMMON_INCLUDE_DIRS} PARENT_SCOPE)
    set(COMMON_LIBS ${COMMON_LIBS} PARENT_SCOPE)
endif()
