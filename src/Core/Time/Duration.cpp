#include "Core/Time/Duration.hpp"
#include "Core/Debug/profiler.hpp"

namespace Raindrop::Core::Time{
	Duration::Duration(time duration) : _duration{duration}{
		RAINDROP_profile_function();
	}

	Duration::Duration(const Duration &other) : _duration{other._duration}{
		RAINDROP_profile_function();
	}

	Duration::Duration(Duration &&other) : _duration{other._duration}{
		RAINDROP_profile_function();
	}

	time Duration::duration() const{
		RAINDROP_profile_function();
		return _duration;
	}
	
	Duration Duration::operator*(int32 x){
		RAINDROP_profile_function();
		return Duration(_duration * x);
	}

	Duration Duration::operator/(int32 x){
		RAINDROP_profile_function();
		return Duration(_duration / x);
	}

	Duration Duration::operator+(const Duration &other){
		RAINDROP_profile_function();
		return Duration(_duration + other._duration);
	}

	Duration Duration::operator-(const Duration &other){
		RAINDROP_profile_function();
		return Duration(_duration - other._duration);
	}

	Duration& Duration::operator*=(int32 x){
		RAINDROP_profile_function();
		_duration *= x;
		return *this;
	}

	Duration& Duration::operator/=(int32 x){
		RAINDROP_profile_function();
		_duration /= x;
		return *this;
	}

	Duration& Duration::operator+=(const Duration &other){
		RAINDROP_profile_function();
		_duration += other._duration;
		return *this;
	}

	Duration& Duration::operator-=(const Duration &other){
		RAINDROP_profile_function();
		_duration -= other._duration;
		return *this;
	}
}