cmake_minimum_required(VERSION 2.8.4)
project(freetype_sample)

if(ANDROID)
  include("$ENV{ANDROID_CMAKE_HOME}/android.apk.cmake" REQUIRED)
endif()

# compiler flags
set(CMAKE_CXX_FLAGS
  "-std=c++11 -ffor-scope -fno-exceptions -pipe -ffunction-sections -fdata-sections -ffast-math -pedantic -Werror -Wnon-virtual-dtor -Wsign-promo -fvisibility=hidden -fvisibility-inlines-hidden -Wstrict-null-sentinel -Os -funroll-all-loops -fpeel-loops -ftree-vectorize")

include_directories(src)

# for android include native_app_glue
if(ANDROID)
  include_directories($ENV{ANDROID_NDK_HOME}/sources/android/native_app_glue)
endif()

set(SOURCE_FILES
    src/Main.cpp
    src/Game.cpp
    src/Log.cpp
    src/Log.h
    src/OpenGL.h
    src/Game.h
    src/Dimension.h
    src/Time.h)

if(ANDROID)
  list(APPEND SOURCE_FILES
    src/EGLWindow.cpp
    src/Activity.h
    src/Activity.cpp
    $ENV{ANDROID_NDK_HOME}/sources/android/native_app_glue/android_native_app_glue.c)

  add_library(freetype_sample SHARED ${SOURCE_FILES})
  target_link_libraries(freetype_sample log android EGL GLESv1_CM z)
  set_target_properties(freetype_sample PROPERTIES COMPILE_DEFINITIONS "ANDROID")

  android_create_apk(freetype_sample "org.discordia.prototype.freetype" "${CMAKE_BINARY_DIR}/apk" "${CMAKE_SOURCE_DIR}/libs" "${CMAKE_SOURCE_DIR}/android" "${CMAKE_SOURCE_DIR}/assets")
else()
  list(APPEND SOURCE_FILES
    src/GLWindow.cpp
    src/Application.cpp
    src/Application.h)

  # GLFW
  find_package(PkgConfig REQUIRED)
  pkg_search_module(GLFW REQUIRED glfw3)
  include_directories(${GLFW_INCLUDE_DIRS})

  # GLEW
  set(GLEW_SHARED_LIBRARY "/usr/lib/x86_64-linux-gnu/libGLEW.so")

  add_executable(freetype_sample ${SOURCE_FILES})
  target_link_libraries(freetype_sample ${GLEW_SHARED_LIBRARY} ${GLFW_STATIC_LIBRARIES})

  # Copy assets
  add_custom_command(TARGET freetype_sample PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/assets")
  add_custom_command(TARGET freetype_sample PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/assets/")
  add_custom_command(TARGET freetype_sample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets/")
endif()