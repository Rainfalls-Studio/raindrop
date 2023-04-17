#include "Core/Time/Timepoint.hpp"
#include "Core/Debug/profiler.hpp"

namespace Raindrop::Core::Time{	
	Timepoint::Timepoint(const Timepoint &other) : _time{other._time}{
		RAINDROP_profile_function();
	}

	Timepoint::Timepoint(Timepoint &&other) : _time{other._time}{
		RAINDROP_profile_function();
	}

	Timepoint::Timepoint(tick time) : _time{time}{
		RAINDROP_profile_function();
	}

	Timepoint::~Timepoint(){
		RAINDROP_profile_function();
	}

	tick Timepoint::time() const{
		RAINDROP_profile_function();
		return _time;
	}

	Duration Timepoint::operator-(const Timepoint &other){
		RAINDROP_profile_function();
		return Duration(static_cast<tick>(_time) - static_cast<tick>(other._time));
	}

	Timepoint Timepoint::now(){
		RAINDROP_profile_function();
		return Timepoint(ticks());
	}
}