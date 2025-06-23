#pragma once

#include "WindowEvents.hpp"
#include "../Input/Key.hpp"

namespace Raindrop::Platform::Event{
	class KeyEvent : public WindowEvent{
		public:
			KeyEvent(Window::Window window, const Input::Key& key) : WindowEvent(window), _key{key}{}
			~KeyEvent() = default;

			const Input::Key& getKey() const noexcept{
				return _key;
			}

		protected:
			Input::Key _key;
	};


	class KeyDown : public KeyEvent{
		public:
			KeyDown(Window::Window window, const Input::Key& key, const std::uint32_t& repeat) : KeyEvent(window, key), _repeat{repeat}{}
			~KeyDown() = default;

			const std::uint32_t& getRepeat() const noexcept{
				return _repeat;
			}

		private:
			std::uint32_t _repeat;
	};

	class KeyUp : public KeyEvent{
		public:
			KeyUp(Window::Window window, const Input::Key& key) : KeyEvent(window, key){}
			~KeyUp() = default;
	};
}