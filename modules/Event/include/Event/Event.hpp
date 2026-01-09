#pragma once

#include <Layers/Transmission.hpp>

namespace Event{
	class Event : public Layers::Transmission{
		public:
			Event() = default;
			virtual ~Event() = default;
	};
}