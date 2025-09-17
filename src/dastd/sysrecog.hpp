/*!
@author Davide Achilli
@copyright Apache 2.0 License
@date 11-AUG-2008

@page sysreco System Recognition Module

SYSTEM RECOGNITION
==================
Recognizes some features of the current compilation environment

System type
-----------
According to the system type and compilation environment, it defines:

 Macro name      | Description
-----------------|-----------------------------------------------------------------
 `DASTD_WIN`     | on Windows (including Windows embedded) with Microsoft compiler
 `DASTD_WINGCC`  | on Windows with GCC compiler
 `DASTD_WINMSC`  | on Windows with Microsoft compiler
 `DASTD_WINEMB`  | on Windows Embedded
 `DASTD_WINANY`  | on Windows (including Windows embedded) with any compiler
 `DASTD_UNIX`    | on all flavors of Unix (including Linux and Cygwin)
 `DASTD_LINUX`   | on Linux systems
 `DASTD_MAC`     | on Mac OS and other Apple derivatives (like also iOS)
 `DASTD_IOS`     | on Apple iOS
 `DASTD_JNI`     | on systems having Java JNI (e.g. Android)
 `DASTD_ANDROID` | on Android
 `DASTD_WASM`    | on Emiscripten WebAssembly
 `DASTD_ESP32`   | on IDF ESP-32


Threading mode
--------------

Endianity
---------
 Macro name             | Description
------------------------|-----------------------------------------------------------------
 `DASTD_LITTLE_ENDIAN`  | on little endian systems
 `DASTD_BIG_ENDIAN`     | on big endian systems


Pointer size
------------
 Macro name             | Description
------------------------|-----------------------------------------------------------------
 `DASTD_POINTER_32`     | pointers are 32-bit
 `DASTD_POINTER_64`     | pointers are 64-bit


Case sensitiveness of paths
---------------------------

 Macro name                      | Description
---------------------------------|-----------------------------------------------------------------
 `DASTD_PATH_CASE_SENSITIVE <n>` |  paths are case: 0=insensitive 1=sensitive


C++ VERSION
-----------
The macro `DASTD_CPP_VER` can be:

 Value  | Description
--------|-----------------------------------------------------------------
	1     | for C++98 + TR1 and less
 11     | for C++11
 14     | for C++14
 17     | for C++17
 20     | for C++20

**/
#pragma once

// Required to avoid Windows.h to define the "min" and the "max" macros
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef NDEBUG
#define DASTD_IS_DEBUG
#define DASTD_DEBUG(a) a
#define DASTD_NDEBUG(a)
#define DASTD_DEBUG_COMMA(a) ,a
#define DASTD_DEBUG_COMMA_POST(a) a,
#else
#define DASTD_DEBUG(a)
#define DASTD_NDEBUG(a) a
#define DASTD_DEBUG_COMMA(a)
#define DASTD_DEBUG_COMMA_POST(a)
#endif
#define DASTD_NOWARN_UNUSED(a) ((void)(a))
#define UN(a) ((void)(a))

#if (defined _MSC_VER)
	#define DASTD_WINMSC
	#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
	#endif
#endif
#if (defined __CYGWIN__)
	#define DASTD_WINGCC 1
	#define DASTD_WINANY 1
#endif
#if (defined __EMSCRIPTEN__)
	#define DASTD_WASM
	#define DASTD_PATH_CASE_SENSITIVE 1
	#define DASTD_LITTLE_ENDIAN 1
#elif (defined WIN32) || (defined __WIN32__) || (defined _WIN32) || (defined MSC_EMBEDDED)
	#ifndef WIN32
		#define WIN32
	#endif
	#ifndef _WIN32
		#define _WIN32
	#endif
	#define DASTD_WIN 1
	#define DASTD_WINANY 1
	#define DASTD_LITTLE_ENDIAN 1
	#define DASTD_PATH_CASE_SENSITIVE 0

#elif (defined unix || defined __unix__)
	#define DASTD_UNIX
	#define DASTD_PATH_CASE_SENSITIVE 1
	#if defined linux || defined __linux
		#define DASTD_LINUX 1
		#include <endian.h>
		#if (__BYTE_ORDER == __LITTLE_ENDIAN)
			#define DASTD_LITTLE_ENDIAN
		#elif (__BYTE_ORDER == __BIG_ENDIAN)
			#define DASTD_BIG_ENDIAN
		#else
			#error "CAN'T DETERMINE ENDIANITY"
		#endif
	#elif defined __CYGWIN__
		 #define DASTD_LITTLE_ENDIAN
	#elif defined sun
		 #define DASTD_BIG_ENDIAN
	#else
		#error "CAN'T DETERMINE WHICH UNIX SYSTEM IS THIS"
	#endif
	#ifndef unix
		#define unix
	#endif
#elif defined __APPLE__
	#include <TargetConditionals.h>
	#define DASTD_MAC 1
	#if TARGET_IPHONE_SIMULATOR
		// iOS Simulator
		#define DASTD_IOS 1
	#elif TARGET_OS_IPHONE
		// iOS device
		#define DASTD_IOS 1
	#elif TARGET_OS_MAC
		// Other kinds of Mac OS
	#else
	#   error "Unknown Apple platform"
	#endif
	#define DASTD_PATH_CASE_SENSITIVE 1
#elif defined ESP_PLATFORM
	#define DASTD_ESP32
	#define DASTD_PATH_CASE_SENSITIVE 1
	#define DASTD_LITTLE_ENDIAN 1
	// ESP-32 does not have thread id
	#define DASTD_DISABLE_THREAD_ID 1
#else
	#error "SYSTEM UNRECOGNIZED!"
#endif

#ifdef __ANDROID__
#define DASTD_ANDROID
#define DASTD_JNI
#endif

#if defined _WIN64 || defined __x86_64__ || defined __LP64__
	#define DASTD_POINTER_64
#else
	#define DASTD_POINTER_32
#endif

/* Try with GCC macros */
#if !defined DASTD_LITTLE_ENDIAN && !defined DASTD_BIG_ENDIAN
	#ifdef __LITTLE_ENDIAN__
		#define DASTD_LITTLE_ENDIAN
	#endif
#endif

/* For VC9 */
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef __GNUC__
	#define DASTD_TYPENAME
#else
	#define DASTD_TYPENAME typename
#endif

#if defined MSC_EMBEDDED
	#define DASTD_HAS_SYS_STAT 0
	#define DASTD_HAS_ERRNO 0
#else
	#define DASTD_HAS_SYS_STAT 1
	#define DASTD_HAS_ERRNO 1
#endif

#ifdef __ANDROID__
#define DASTD_ANDROID
#define DASTD_JNI
#endif

#ifdef _MSVC_LANG
	#if _MSVC_LANG >= 202002L
		#define DASTD_CPP_VER 20
	#elif _MSVC_LANG >= 201703L
		#define DASTD_CPP_VER 17
	#elif _MSVC_LANG >= 201402L
		#define DASTD_CPP_VER 14
	#elif _MSVC_LANG >= 201103L
		#define DASTD_CPP_VER 11
	#else
		#define DASTD_CPP_VER 1
	#endif
#elif defined __cplusplus
	#if __cplusplus >= 202002L
		#define DASTD_CPP_VER 20
	#elif __cplusplus >= 201703L
		#define DASTD_CPP_VER 17
	#elif __cplusplus >= 201402L
		#define DASTD_CPP_VER 14
	#elif __cplusplus >= 201103L
		#define DASTD_CPP_VER 11
	#else
		#define DASTD_CPP_VER 1
	#endif
#endif
#ifndef DASTD_CPP_VER
#define DASTD_CPP_VER 1
#endif

namespace dastd {
#if defined DASTD_LITTLE_ENDIAN
static constexpr bool g_is_little_endian = true;
#elif defined DASTD_BIG_ENDIAN
static constexpr bool g_is_little_endian = false;
#else
#error "Neither DASTD_LITTLE_ENDIAN nor is DASTD_BIG_ENDIAN"
#endif
} // namespace dastd

#ifndef DASTD_WASM
#define EMSCRIPTEN_KEEPALIVE
#else
#include <emscripten/emscripten.h>
#endif
