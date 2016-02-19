Idea
======================================
This is suppose to be an example of opengl rendering text with freetype2 on both desktop (linux, osx and windows) and
mobile (android and ios). On mobile it should use opengl es 2.0. On desktop we use GLFW and GLEW to abstract away the
differences between desktop os.

Current status
========================================================
Builds on Linux Mint and Android.
Can read font and draw text.


Building
========================================================

Build instructions using cmake on various platforms. Note that I'm developing 
on Linux Mint and the instruction do probably have to change a bit if using 
Windows or OSX but should be similar.

Directly after clone do:

* git submodule init
* git submodule update

To clone freetype2 into the submodule under deps/

**Desktop**

I have only built it on Linux Mint but should be trivial to build on Windows and OSX 
since I'm using GLFW3 and GLEW to abstract away OS differences. But that might be 
famous last words.

To get it to build you need binaries of GLEW and GLFW3 and make sure that they are 
declared with correct paths in CMakeLists.txt. Right now they are setup where they 
install from the package manager on Linux Mint.

To build do (after clone):


* mkdir -p build/<os> (i.e. mkdir -p build/linux)
* cd build/linux
* cmake ../..
* make -j8

Then run with: ./freetype_sample

**Android**

First of you need to install an Android SDK and NDK.

Then you need to setup android-cmake, see: 

* https://github.com/Discordia/android-cmake
* https://github.com/Discordia/android-cmake-example

To build:

* mkidr -p build/android
* cd build/android
* cmake -DCMAKE_TOOLCHAIN_FILE=/usr/local/android/android-cmake/android.toolchain.cmake ../..
* make -j8

Then install it on a device:

adb install -r apk/bin/freetype_sample-debug.apk

The installed app will be called "Freetype prototype" and have the default android icon.

**IOS**

Not implemented (yet).