#ifndef __RAINDROP_CORE_DEBUG_PROFILER_HPP__
#define __RAINDROP_CORE_DEBUG_PROFILER_HPP__

#include "Core/core.hpp"
#include "Core/common.hpp"
#include "Core/module.hpp"

namespace Raindrop::Core::Debug::Profiler{
	struct Profile{
		Profile(const char* scope);
		~Profile();
	};

	void beginProfile(const char* name, const char* filepath);
	void endProfile();
	void profile(bool profile = true);

	void init();
	void shutdown();

	class __Profiler;
}

// internal
RAINDROP_MODULE void __RAINDROP_profile_setContext(Raindrop::Core::Debug::Profiler::__Profiler* profiler);
RAINDROP_MODULE Raindrop::Core::Debug::Profiler::__Profiler* __RAINDROP_profile_getContext();

#define RAINDROP_COMBINE1(X,Y) X##Y  // helper macro
#define RAINDROP_COMBINE(X,Y) RAINDROP_COMBINE1(X,Y)

#ifdef RAINDROP_PROFILE
	#define RAINDROP_profile_begin(name, filepath) ::Raindrop::Core::Debug::Profiler::beginProfile(name, filepath)
	#define RAINDROP_profile_end() ::Raindrop::Core::Debug::Profiler::endProfile()
	#define RAINDROP_profile_scope(name) RAINDROP_COMBINE(::Raindrop::Core::Debug::Profiler::Profile __profile, __LINE__)(name)
	#define RAINDROP_profile_function() RAINDROP_profile_scope(__PRETTY_FUNCTION__)
	#define RAINDROP_profile(_profile) Raindrop::Core::Debug::Profiler::profile(_profile)
#else
	#define RAINDROP_profile_begin(name, filepath)
	#define RAINDROP_profile_end()
	#define RAINDROP_profile_scope(name)
	#define RAINDROP_profile_function()
	#define RAINDROP_profile()
#endif

#endif