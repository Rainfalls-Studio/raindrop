#include "Core/Time/Duration.hpp"
#include "Core/Debug/profiler.hpp"

namespace Raindrop::Core::Time{
	Duration::Duration(time64 duration) : _duration{duration}{
		RAINDROP_profile_function();
	}

	Duration::Duration(const Duration &other) : _duration{other._duration}{
		RAINDROP_profile_function();
	}

	Duration::Duration(Duration &&other) : _duration{other._duration}{
		RAINDROP_profile_function();
	}

	time64 Duration::duration() const{
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

	timef64 Duration::toNanoseconds() const{
		RAINDROP_profile_function();
		return Time::toNanoseconds(_duration);
	}

	timef64 Duration::toMicroseconds() const{
		RAINDROP_profile_function();
		return Time::toMicroseconds(_duration);
	}

	timef64 Duration::toMilliseconds() const{
		RAINDROP_profile_function();
		return Time::toMilliseconds(_duration);
	}

	timef64 Duration::toSeconds() const{
		RAINDROP_profile_function();
		return Time::toSeconds(_duration);
	}

	timef64 Duration::toMinutes() const{
		RAINDROP_profile_function();
		return Time::toMinutes(_duration);
	}

	timef64 Duration::toHours() const{
		RAINDROP_profile_function();
		return Time::toHours(_duration);
	}

	timef64 Duration::toDays() const{
		RAINDROP_profile_function();
		return Time::toDays(_duration);
	}
}