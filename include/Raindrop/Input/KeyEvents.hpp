#pragma once

#include "Key.hpp"
#include "../Event/Event.hpp"
#include <cstdint>

namespace Raindrop::Input{
	class KeyEvent : public Event::Event{
		public:
			KeyEvent(const Key& key) : _key{key}{}
			~KeyEvent() = default;

			const Key& getKey() const noexcept{
				return _key;
			}

		protected:
			Key _key;
	};


	class KeyDown : public KeyEvent{
		public:
			KeyDown(const Key& key, const std::uint32_t& repeat) : KeyEvent(key), _repeat{repeat}{}
			~KeyDown() = default;

			const std::uint32_t& getRepeat() const noexcept{
				return _repeat;
			}

		private:
			std::uint32_t _repeat;
	};

	class KeyUp : public KeyEvent{
		public:
			KeyUp(const Key& key) : KeyEvent(key){}
			~KeyUp() = default;
	};
}