#ifndef __RAINDROP_CORE_COMMON_HPP__
#define __RAINDROP_CORE_COMMON_HPP__

#include "Core/core.hpp"

#if! defined(__SIZEOF_INT128__)
	#warning int 128 is not defined, may cause issues.
	typedef long long int __int128;
#endif

namespace Raindrop{
	typedef char int8;
	typedef short int int16;
	typedef int int32;
	typedef long long int int64;
	typedef __int128  int128;
	typedef float float32;
	typedef long double float64;
	typedef unsigned char uint8;
	typedef unsigned short int uint16;
	typedef unsigned int uint32;
	typedef unsigned long long int uint64;
	typedef unsigned __int128 uint128;

	#ifdef RAINDROP_64
		typedef unsigned long long int usize;
		typedef unsigned long long int uptr;
	#else
		typedef unsigned int usize;
		typedef unsigned int uptr;
	#endif

	typedef uint32  ID32;
	typedef uint64  ID64;
	typedef uint128 ID128;

	struct version{
		uint16 major;
		uint16 minor;
		uint16 patch;
	};
}


#define RAINDROP_CreateVersion(major, minor, patch) ::Raindrop::version{major, minor, patch};
#define RAINDROP_CORE_VERSION RAINDROP_CreateVersion(RAINDROP_VERSION_MAJOR, RAINDROP_VERSION_MINOR, RAINDROP_VERSION_PATCH)

#endif