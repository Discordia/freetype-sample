cmake_minimum_required(VERSION 2.8.4)
project(freetype_sample)

if(ANDROID)
  include("$ENV{ANDROID_CMAKE_HOME}/android.apk.cmake" REQUIRED)
endif()

# compiler flags
set(CMAKE_CXX_FLAGS
        "-std=c++11 -ffor-scope -fno-exceptions -pipe -ffunction-sections -fdata-sections -ffast-math -pedantic -Werror -Wnon-virtual-dtor -Wsign-promo -fvisibility=hidden -fvisibility-inlines-hidden -Os -ftree-vectorize")

# Dependencies
add_subdirectory(deps/freetype)
add_subdirectory(deps/app-framework/src/core)

# Includes
include_directories(src deps/freetype/include deps/app-framework/src)

# for android include native_app_glue
if(ANDROID)
  include_directories($ENV{ANDROID_NDK_HOME}/sources/android/native_app_glue)
endif()

set(SOURCE_FILES
        src/FontPrototype.cpp
        src/font/FontAtlas.cpp
        src/font/FontBatchRenderer.cpp
        src/font/FTFont.cpp
        src/font/FTFontChar.cpp
        src/font/FontGeometry.h
        src/font/VertexTypes.h
        src/font/BinPacker.cpp
        src/font/BinPacker.h)

if(ANDROID)
  add_library(freetype_sample SHARED ${SOURCE_FILES})
  target_link_libraries(freetype_sample app_framework_library freetype log android EGL GLESv2 z)
  set_target_properties(freetype_sample PROPERTIES COMPILE_DEFINITIONS "ANDROID")

  android_create_apk(freetype_sample "org.discordia.prototype.freetype" "${CMAKE_BINARY_DIR}/apk" "${CMAKE_SOURCE_DIR}/libs" "${CMAKE_SOURCE_DIR}/android" "${CMAKE_SOURCE_DIR}/assets")
elseif(APPLE)
  # GLFW
  find_package(glfw3 REQUIRED)
  include_directories(${GLFW_INCLUDE_DIRS})

  find_package(GLEW REQUIRED)
  include_directories(${GLEW_INCLUDE_DIRS})

  # app_framework executable
  add_executable(freetype_sample ${SOURCE_FILES})
  target_link_libraries(freetype_sample app_framework_library freetype ${GLEW_LIBRARY} glfw3)
  target_link_libraries(freetype_sample "-framework OpenGL")
else()
  # GLFW
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLFW REQUIRED glfw3)
  include_directories(${GLFW_INCLUDE_DIRS})

  # GLEW
  set(GLEW_SHARED_LIBRARY "/usr/lib/x86_64-linux-gnu/libGLEW.so")

  add_executable(freetype_sample ${SOURCE_FILES})
  target_link_libraries(freetype_sample app_framework_library ${GLEW_SHARED_LIBRARY} freetype ${GLFW_STATIC_LIBRARIES})
endif()

if (NOT ANDROID)
  # Copy assets
  add_custom_command(TARGET freetype_sample PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/assets")
  add_custom_command(TARGET freetype_sample PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets/")
  add_custom_command(TARGET freetype_sample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets/")
endif()