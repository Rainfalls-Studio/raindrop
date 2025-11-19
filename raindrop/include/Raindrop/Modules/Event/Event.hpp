#pragma once

#include <Raindrop/Modules/Layers/Transmission.hpp>

namespace Raindrop::Event{
	class Event : public Layers::Transmission{
		public:
			Event() = default;
			virtual ~Event() = default;
	};
}