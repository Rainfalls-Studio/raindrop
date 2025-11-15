#pragma once

#include <Raindrop/Modules/Layers/Transmission.hpp>

namespace Raindrop::Event{
	class Event : public Layers::Transmission<Event>{
		public:
			Event() = default;
			virtual ~Event() = default;
	};
}