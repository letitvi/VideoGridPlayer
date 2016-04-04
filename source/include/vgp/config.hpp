
#pragma once
#ifndef VGP_CONFIG_HPP
#define VGP_CONFIG_HPP


////////////////////////////////////////////////////////////
// Identify the operating system
// see http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
////////////////////////////////////////////////////////////

/* Define the version */
#define VGP_VERSION_MAJOR 1
#define VGP_VERSION_MINOR 0
#define VGP_VERSION_PATCH 0


#if defined(_WIN32)

// Windows
#define VGP_SYSTEM_WINDOWS

#ifndef NOMINMAX
#define NOMINMAX
#endif

#elif defined(__APPLE__) && defined(__MACH__)

// Apple platform, see which one it is
#include "TargetConditionals.h"

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

// iOS
#define VGP_SYSTEM_IOS

#elif TARGET_OS_MAC

// MacOS
#define VGP_SYSTEM_MACOS

#else

// Unsupported Apple system
#error This Apple operating system is not supported by this library

#endif

#elif defined(__unix__)
// UNIX system, see which one it is
#if defined(__ANDROID__)

// Android
#define VGP_SYSTEM_ANDROID

#elif defined(__linux__)

// Linux
#define VGP_SYSTEM_LINUX

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

// FreeBSD
#define VGP_SYSTEM_FREEBSD

#else

// Unsupported UNIX system
#error This UNIX operating system is not supported by this library

#endif
#else
// Unsupported system
#error This operating system is not supported by this library
#endif


////////////////////////////////////////////////////////////
// Define a portable debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG)

#define VGP_DEBUG

#endif


#if !defined(VGP_STATIC)

#if defined(VGP_SYSTEM_WINDOWS)

// Windows compilers need specific (and different) keywords for export and import
#define VGP_API_EXPORT __declspec(dllexport)
#define VGP_API_IMPORT __declspec(dllimport)

// For Visual C++ compilers, we also need to turn off this annoying C4251 warning
#ifdef _MSC_VER

//#pragma warning(disable: 4251)

#endif

#else // Linux, FreeBSD, Mac OS X

#if __GNUC__ >= 4

// GCC 4 has special keywords for showing/hidding symbols,
// the same keyword is used for both importing and exporting
#define VGP_API_EXPORT __attribute__ ((__visibility__ ("default")))
#define VGP_API_IMPORT __attribute__ ((__visibility__ ("default")))

#else

// GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
#define VGP_API_EXPORT
#define VGP_API_IMPORT

#endif

#endif


#else

// Static build doesn't need import/export macros
#define VGP_API_EXPORT
#define VGP_API_IMPORT

#endif


////////////////////////////////////////////////////////////
// Define portable fixed-size types
////////////////////////////////////////////////////////////
namespace vgp
{
	// All "common" platforms use the same size for char, short and int
	// (basically there are 3 types for 3 sizes, so no other match is possible),
	// we can use them without doing any kind of check

	// 8 bits integer types
	typedef signed   char Int8;
	typedef unsigned char Uint8;

	// 16 bits integer types
	typedef signed   short Int16;
	typedef unsigned short Uint16;

	// 32 bits integer types
	typedef signed   int Int32;
	typedef unsigned int Uint32;

	// 64 bits integer types
#if defined(_MSC_VER)
	typedef signed   __int64 Int64;
	typedef unsigned __int64 Uint64;
#else
	typedef signed   long long Int64;
	typedef unsigned long long Uint64;
#endif

} // namespace oscar



#endif // OSCAR_CONFIG_HPP
