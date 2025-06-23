#pragma once

#include "../Input/MouseButton.hpp"
#include "WindowEvents.hpp"

namespace Raindrop::Platform::Event{

	class MouseMovedEvent : public WindowEvent{
		public:
			MouseMovedEvent(Window::Window window, const glm::u32vec2& position, const glm::i32vec2& delta) :
				WindowEvent(window),
				_position{position},
				_delta{delta}
			{}

			~MouseMovedEvent() = default;

			const glm::u32vec2& getPosition() const noexcept{
				return _position;
			}

			const glm::i32vec2& getDelta() const noexcept{
				return _delta;
			}

		protected:
			glm::u32vec2 _position;
			glm::i32vec2 _delta;
	};

	class MouseButtonDownEvent : public WindowEvent{
		public:
			MouseButtonDownEvent(Window::Window window, const Input::MouseButton& button, const std::uint32_t& clicks, const glm::u32vec2& position) : 
				WindowEvent(window),
				_button{button},
				_clicks{clicks},
				_position{position}
			{}

			~MouseButtonDownEvent() = default;

			const Input::MouseButton& getButton() const noexcept{
				return _button;
			}

			const std::uint32_t& getClicks() const noexcept{
				return _clicks;
			}

			const glm::u32vec2& getPosition() const noexcept{
				return _position;
			}

		private:
			Input::MouseButton _button;
			std::uint32_t _clicks;
			glm::u32vec2 _position;
	};

	class MouseButtonUpEvent : public WindowEvent{
		public:
			MouseButtonUpEvent(Window::Window window, const Input::MouseButton& button, const glm::u32vec2& position) : 
				WindowEvent(window),
				_button{button},
				_position{position}
			{}

			~MouseButtonUpEvent() = default;

			const Input::MouseButton& getButton() const noexcept{
				return _button;
			}

			const glm::u32vec2& getPosition() const noexcept{
				return _position;
			}

		private:
			Input::MouseButton _button;
			glm::u32vec2 _position;
	};

	class MouseScrollEvent : public WindowEvent{
		public:
			MouseScrollEvent(Window::Window window, const float& y) :
				WindowEvent(window),
				_y{y}
			{}

			const float& getY() const noexcept{
				return _y;
			}

		private:
			float _y;
	};
}