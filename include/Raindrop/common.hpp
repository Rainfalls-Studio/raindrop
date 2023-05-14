#ifndef __RAINDROP_COMMON_HPP__
#define __RAINDROP_COMMON_HPP__

#include <filesystem>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <utility>
#include <cstring>
#include <new>

#define RAINDROP_VERSION_MAJOR 0
#define RAINDROP_VERSION_MINOR 1
#define RAINDROP_VERSION_PATCH 0

#if defined(_WIN32) || defined(__CYGWIN__)
	#define RAINDROP_WINDOWS
	#if defined(_WIN64)
		#define RAINDROP_64
	#else
		#define RAINDROP_32
	#endif

	#ifdef RAINDROP_DLL
		#ifdef RAINDROP_EXPORT
			#define RAINDROP_API __declspec(dllexport)
		#else
			#define RAINDROP_API __declspec(dllimport)
		#endif
	#else
		#define RAINDROP_API
	#endif
#elif defined(unix) || defined(__unix__) || defined(__unix)
	#define RAINDROP_UNIX

	#ifdef RAINDROP_DLL
		#define RAINDROP_API __attribute__((visibility("default")))
	#else
		#define RAINDROP_API
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#define RAINDROP_APPLE
#elif defined(__linux__)
	#define RAINDROP_LINUX
	
	#ifdef RAINDROP_DLL
		#define RAINDROP_API __attribute__((visibility("default")))
	#else
		#define RAINDROP_API
	#endif
#elif defined(__FreeBSD__)
	#define RAINDRPO_FREE_BSD
#elif defined(__ANDROID__)
	#define RAINDROP_ANDROID
#endif

#if !defined(__SIZEOF_INT128__)
	#warning int 128 is not defined.
	typedef long long int __int128[2];
#endif

#if defined(_WIN32)
	#define RAINDROP_MODULE extern "C" __declspec(dllexport) __stdcall
#else
	#define RAINDROP_MODULE extern "C" __attribute__((visibility("default"))) __stdcall
#endif

#define RAINDROP_CreateVersion(major, minor, patch) ::Raindrop::Version{major, minor, patch}
#define RAINDROP_VERSION RAINDROP_CreateVersion(RAINDROP_VERSION_MAJOR, RAINDROP_VERSION_MINOR, RAINDROP_VERSION_PATCH)

#define RAINDROP_PACKED __attribute__((__packed__))
#define RAINDROP_ALIGNED(n) __attribute__((__aligned__(n)))
#define RAINDROP_NORETURN __attribute__((__noreturn__))
#define RAINDROP_CONSTRUCTOR __attribute__((__constructor__))
#define RAINDROP_DEPRECATED __attribute__((__deprecated__))
#define RAINDORP_UNSUED __attribute__((__unused__))

namespace Raindrop{
	typedef char int8;
	typedef short int int16;
	typedef int int32;
	typedef long long int int64;
	typedef __int128  int128;
	
	typedef unsigned char uint8;
	typedef unsigned short int uint16;
	typedef unsigned int uint32;
	typedef unsigned long long int uint64;
	typedef unsigned __int128 uint128;

	typedef float float32;
	typedef long double float64;

	#ifdef RAINDROP_64
		typedef unsigned long long int usize;
		typedef unsigned long long int uptr;
	#else
		typedef unsigned int usize;
		typedef unsigned int uptr;
	#endif

	// ID types are used to as an index in high level. For arrays it is better to use usize.
	typedef uint32 ID32;
	typedef uint64 ID64;

	// INVALID_ID<SIZE> represent an invalid indentifier. It is the maximal possible value
	static constexpr ID32 INVALID_ID32 = static_cast<ID32>(~(0U));
	static constexpr ID64 INVALID_ID64 = static_cast<ID64>(~(0ULL));

	struct RAINDROP_PACKED Version{
		uint16 major;
		uint16 minor;
		uint16 patch;

		Version() = default;
		Version(uint16 major, uint16 minor, uint16 patch) : major{major}, minor{minor}, patch{patch}{}
		
		bool operator==(const Version& other) const{
			return major == other.major && minor == other.minor && patch == other.patch;
		}
	};

	template <typename... Args>
	struct TotalSize;

	template <>
	struct TotalSize<>{
		static constexpr size_t value = 0;
	};

	template <typename T, typename... Args>
	struct TotalSize<T, Args...>{
		static constexpr size_t value = sizeof(T) + TotalSize<Args...>::value;
	};
}

#endif