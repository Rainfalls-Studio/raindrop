#include "Core/Time/Timepoint.hpp"
#include "Core/Debug/profiler.hpp"

namespace Raindrop::Core::Time{	
	RAINDROP_API Timepoint::Timepoint(const Timepoint &other) : _time{other._time}{
		RAINDROP_profile_function();
	}

	RAINDROP_API Timepoint::Timepoint(Timepoint &&other) : _time{other._time}{
		RAINDROP_profile_function();
	}

	RAINDROP_API Timepoint::Timepoint(tick time) : _time{time}{
		RAINDROP_profile_function();
	}

	RAINDROP_API Timepoint::~Timepoint(){
		RAINDROP_profile_function();
	}

	RAINDROP_API tick Timepoint::time() const{
		RAINDROP_profile_function();
		return _time;
	}

	RAINDROP_API Duration Timepoint::operator-(const Timepoint &other){
		RAINDROP_profile_function();
		return Duration(static_cast<tick>(_time) - static_cast<tick>(other._time));
	}

	RAINDROP_API Timepoint Timepoint::now(){
		RAINDROP_profile_function();
		return Timepoint(ticks());
	}
}