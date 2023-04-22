#include "Core/Time/Duration.hpp"
#include "Core/Debug/profiler.hpp"

namespace Raindrop::Core::Time{
	RAINDROP_API Duration::Duration(time duration) : _duration{duration}{
		RAINDROP_profile_function();
	}

	RAINDROP_API Duration::Duration(const Duration &other) : _duration{other._duration}{
		RAINDROP_profile_function();
	}

	RAINDROP_API Duration::Duration(Duration &&other) : _duration{other._duration}{
		RAINDROP_profile_function();
	}

	RAINDROP_API time Duration::duration() const{
		RAINDROP_profile_function();
		return _duration;
	}
	
	RAINDROP_API Duration Duration::operator*(int32 x){
		RAINDROP_profile_function();
		return Duration(_duration * x);
	}

	RAINDROP_API Duration Duration::operator/(int32 x){
		RAINDROP_profile_function();
		return Duration(_duration / x);
	}

	RAINDROP_API Duration Duration::operator+(const Duration &other){
		RAINDROP_profile_function();
		return Duration(_duration + other._duration);
	}

	RAINDROP_API Duration Duration::operator-(const Duration &other){
		RAINDROP_profile_function();
		return Duration(_duration - other._duration);
	}

	RAINDROP_API Duration& Duration::operator*=(int32 x){
		RAINDROP_profile_function();
		_duration *= x;
		return *this;
	}

	RAINDROP_API Duration& Duration::operator/=(int32 x){
		RAINDROP_profile_function();
		_duration /= x;
		return *this;
	}

	RAINDROP_API Duration& Duration::operator+=(const Duration &other){
		RAINDROP_profile_function();
		_duration += other._duration;
		return *this;
	}

	RAINDROP_API Duration& Duration::operator-=(const Duration &other){
		RAINDROP_profile_function();
		_duration -= other._duration;
		return *this;
	}
}