cmake_minimum_required(VERSION 2.8.4)
project(freetype_sample)

if(ANDROID)
  include("$ENV{ANDROID_CMAKE_HOME}/android.apk.cmake" REQUIRED)
endif()

# compiler flags
set(CMAKE_CXX_FLAGS
  "-std=c++11 -ffor-scope -fno-exceptions -pipe -ffunction-sections -fdata-sections -ffast-math -pedantic -Werror -Wnon-virtual-dtor -Wsign-promo -fvisibility=hidden -fvisibility-inlines-hidden -Wstrict-null-sentinel -Os -funroll-all-loops -fpeel-loops -ftree-vectorize")

# Dependencies
add_subdirectory(deps/freetype)

# Includes
include_directories(src deps/freetype/include)

# for android include native_app_glue
if(ANDROID)
  include_directories($ENV{ANDROID_NDK_HOME}/sources/android/native_app_glue)
endif()

set(SOURCE_FILES
        src/Main.cpp
        src/Game.cpp
        src/core/BufferObject.cpp
        src/core/Dimension.h
        src/core/Log.cpp
        src/core/OpenGL.cpp
        src/core/ShaderObject.cpp
        src/core/ShaderProgram.h
        src/core/Time.h
        src/core/Stream.h
        src/core/StreamFactory.h
        src/font/FontAtlas.cpp
        src/font/FontBatchRenderer.cpp
        src/font/FTFont.cpp
        src/font/FTFontChar.cpp
        src/font/FontGeometry.h
        src/font/VertexTypes.h
        src/font/BinPacker.cpp
        src/font/BinPacker.h)

if(ANDROID)
  list(APPEND SOURCE_FILES
          src/core/android/EGLWindow.cpp
          src/core/android/Activity.cpp
          src/core/android/AndroidStreamFactory.h
          src/core/android/AssetStream.cpp
          src/core/android/AssetStream.h
    $ENV{ANDROID_NDK_HOME}/sources/android/native_app_glue/android_native_app_glue.c)

  add_library(freetype_sample SHARED ${SOURCE_FILES})
  target_link_libraries(freetype_sample freetype log android EGL GLESv2 z)
  set_target_properties(freetype_sample PROPERTIES COMPILE_DEFINITIONS "ANDROID")

  android_create_apk(freetype_sample "org.discordia.prototype.freetype" "${CMAKE_BINARY_DIR}/apk" "${CMAKE_SOURCE_DIR}/libs" "${CMAKE_SOURCE_DIR}/android" "${CMAKE_SOURCE_DIR}/assets")
else()
  list(APPEND SOURCE_FILES
          src/core/desktop/GLWindow.cpp
          src/core/desktop/GLWindow.h
          src/core/desktop/Application.cpp
          src/core/desktop/Application.h
          src/core/desktop/FileStream.h
          src/core/desktop/FileStream.cpp
          src/core/desktop/DesktopStreamFactory.h)

  # GLFW
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLFW REQUIRED glfw3)
  include_directories(${GLFW_INCLUDE_DIRS})

  # GLEW
  set(GLEW_SHARED_LIBRARY "/usr/lib/x86_64-linux-gnu/libGLEW.so")


  add_executable(freetype_sample ${SOURCE_FILES})
  target_link_libraries(freetype_sample ${GLEW_SHARED_LIBRARY} freetype ${GLFW_STATIC_LIBRARIES})

  # Copy assets
  add_custom_command(TARGET freetype_sample PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/assets")
  add_custom_command(TARGET freetype_sample PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets/")
  add_custom_command(TARGET freetype_sample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets/")
endif()