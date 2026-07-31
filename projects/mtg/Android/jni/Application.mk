APP_PROJECT_PATH := $(call my-dir)/..
APP_CPPFLAGS += -frtti -fexceptions
# Codebase predates C++17 (auto_ptr, random_shuffle in bundled Boost/engine);
# modern NDK clang defaults past it. Same pin as the PSP/PC builds.
APP_CPPFLAGS += -std=gnu++14
APP_ABI := arm64-v8a
APP_PLATFORM := android-21
APP_CFLAGS += -march=armv8.1-a
# Vendored SDL (old) uses pre-C99-strict GLES1 prototypes; clang 17+ makes
# implicit declarations a hard error by default.
APP_CFLAGS += -Wno-implicit-function-declaration
APP_CPPFLAGS += -D__ARM_FEATURE_LSE=1
#APP_ABI := x86 # mainly for emulators
APP_STL := c++_static
APP_MODULES := libpng libjpeg main SDL

#APP_OPTIM is 'release' by default
APP_OPTIM := release
