#include "Timepoint.hpp"
#include "../Debug/profiler.hpp"

namespace Raindrop::Core::Time{	
	Timepoint::Timepoint(const Timepoint &other) : _time{other._time}{
		RAINDROP_profile_function();
	}

	Timepoint::Timepoint(Timepoint &&other) : _time{other._time}{
		RAINDROP_profile_function();
	}

	Timepoint::Timepoint(utime64 time) : _time{time}{
		RAINDROP_profile_function();
	}

	Timepoint::~Timepoint(){
		RAINDROP_profile_function();
	}

	utime64 Timepoint::time() const{
		RAINDROP_profile_function();
		return _time;
	}

	Duration Timepoint::operator-(const Timepoint &other){
		RAINDROP_profile_function();
		return Duration((time64)_time - (time64)other._time);
	}

	Timepoint Timepoint::now(){
		RAINDROP_profile_function();
		return Timepoint(getTicks());
	}

	timef64 Timepoint::toNanoseconds() const{
		RAINDROP_profile_function();
		return Time::toNanoseconds(_time);
	}

	timef64 Timepoint::toMicroseconds() const{
		RAINDROP_profile_function();
		return Time::toMicroseconds(_time);
	}

	timef64 Timepoint::toMilliseconds() const{
		RAINDROP_profile_function();
		return Time::toMilliseconds(_time);
	}

	timef64 Timepoint::toSeconds() const{
		RAINDROP_profile_function();
		return Time::toSeconds(_time);
	}

	timef64 Timepoint::toMinutes() const{
		RAINDROP_profile_function();
		return Time::toMinutes(_time);
	}

	timef64 Timepoint::toHours() const{
		RAINDROP_profile_function();
		return Time::toHours(_time);
	}

	timef64 Timepoint::toDays() const{
		RAINDROP_profile_function();
		return Time::toDays(_time);
	}
}