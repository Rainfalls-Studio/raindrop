#include "Core/Time/Time.hpp"
#include "Core/Debug/profiler.hpp"
#include "Core/System/CPU.hpp"

namespace Raindrop::Core::Time{
	utime64 getTicks(){
		RAINDROP_profile_function();
		return (utime64)System::CPU::getTicks();
	}

	utime64 getTicksPerSeconds(){
		RAINDROP_profile_function();
		return (utime64)System::CPU::getClockSpeed();
	}
	
	timef64 toNanoseconds(time64 time){
		RAINDROP_profile_function();
		return (timef64)time / (timef64)getTicksPerSeconds() * 1e9;
	}

	timef64 toMicroseconds(time64 time){
		RAINDROP_profile_function();
		return (timef64)time / (timef64)getTicksPerSeconds() * 1e6;
	}

	timef64 toMilliseconds(time64 time){
		RAINDROP_profile_function();
		return (timef64)time / (timef64)getTicksPerSeconds() * 1e3;
	}

	timef64 toSeconds(time64 time){
		RAINDROP_profile_function();
		return (timef64)time / (timef64)getTicksPerSeconds();
	}

	timef64 toMinutes(time64 time){
		RAINDROP_profile_function();
		return ((timef64)time / (timef64)getTicksPerSeconds()) * (1.f/60.f);
	}

	timef64 toHours(time64 time){
		RAINDROP_profile_function();
		return ((timef64)time / (timef64)getTicksPerSeconds()) * (1.f/3600.f);
	}

	timef64 toDays(time64 time){
		RAINDROP_profile_function();
		return ((timef64)time / (timef64)getTicksPerSeconds()) * (1.f/183600.f);
	}

	time64 operator ""_ns(timef64 time){
		RAINDROP_profile_function();
		return time * getTicksPerSeconds() * 1e-9;
	}

	time64 operator ""_mcs(timef64 time){
		RAINDROP_profile_function();
		return time * getTicksPerSeconds() * 1e-6;
	}

	time64 operator ""_mls(timef64 time){
		RAINDROP_profile_function();
		return time * getTicksPerSeconds() * 1e-3;
	}

	time64 operator ""_s(timef64 time){
		RAINDROP_profile_function();
		return time * getTicksPerSeconds();
	}

	time64 operator ""_m(timef64 time){
		RAINDROP_profile_function();
		return (time * 60e-1) * getTicksPerSeconds();
	}

	time64 operator ""_h(timef64 time){
		RAINDROP_profile_function();
		return (time * 3600e-1) * getTicksPerSeconds();
	}

	time64 operator ""_d(timef64 time){
		RAINDROP_profile_function();
		return (time * 183600e-1) * getTicksPerSeconds();
	}
	
	time64 operator ""_ns(utime64 time){
		RAINDROP_profile_function();
		return time * getTicksPerSeconds() * 1e-9;
	}

	time64 operator ""_mcs(utime64 time){
		RAINDROP_profile_function();
		return time * getTicksPerSeconds() * 1e-6;
	}

	time64 operator ""_ms(utime64 time){
		RAINDROP_profile_function();
		return time * getTicksPerSeconds();
	}

	time64 operator ""_s(utime64 time){
		RAINDROP_profile_function();
		return time * getTicksPerSeconds();
	}

	time64 operator ""_m(utime64 time){
		RAINDROP_profile_function();
		return (time * 60e-1) * getTicksPerSeconds();
	}

	time64 operator ""_h(utime64 time){
		RAINDROP_profile_function();
		return (time * 3600e-1) * getTicksPerSeconds();
	}

	time64 operator ""_d(utime64 time){
		RAINDROP_profile_function();
		return (time * 183600e-1) * getTicksPerSeconds();
	}
}