#ifndef __RAINDROP_CORE_HPP__
#define __RAINDROP_CORE_HPP__

// ! /!\ DO NOT DELETE /!\ 
// ppbrbrprbrpb
// ! /!\ DO NOT DELETE /!\

#include <stdexcept>
#include <typeinfo>

#define RAINDROP_PROFILE

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
#elif defined(unix) || defined(__unix__) || defined(__unix)
	#define RAINDROP_UNIX
#elif defined(__APPLE__) || defined(__MACH__)
	#define RAINDROP_APPLE
#elif defined(__linux__)
	#define RAINDROP_LINUX
#elif defined(__FreeBSD__)
	#define RAINDRPO_FREE_BSD
#elif defined(__ANDROID__)
	#define RAINDROP_ANDROID
#endif

#endif