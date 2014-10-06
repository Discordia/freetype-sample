project (fontstash-sample)

cmake_minimum_required (VERSION 2.8)

# The version number.
set (FONTSTASH_VERSION_MAJOR 1)
set (FONTSTASH_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/FontstashConfig.h.i"
  "${PROJECT_SOURCE_DIR}/FontstashConfig.h"
  )

#include the subdirectory containing our libs
add_subdirectory (fontstash)
include_directories(fontstash/src)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

# add the executable
add_executable(fontstash-sample main.cpp)
target_link_libraries(fontstash-sample fontstash ${GLFW_STATIC_LIBRARIES})
