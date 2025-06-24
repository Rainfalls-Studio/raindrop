#pragma once

#include "MouseButton.hpp"
#include "MousePosition.hpp"
#include "../Event/Event.hpp"
#include <glm/glm.hpp>

namespace Raindrop::Input{

	class MouseMovedEvent : public Event::Event{
		public:
			MouseMovedEvent(const MousePosition& position, const glm::i32vec2& delta) :
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

	class MouseButtonDownEvent : public Event::Event{
		public:
			MouseButtonDownEvent(const Input::MouseButton& button, const std::uint32_t& clicks, const glm::u32vec2& position) : 
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

	class MouseButtonUpEvent : public Event::Event{
		public:
			MouseButtonUpEvent(const Input::MouseButton& button, const glm::u32vec2& position) : 
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

	class MouseScrollEvent : public Event::Event{
		public:
			MouseScrollEvent(const float& y) :
				_y{y}
			{}

			const float& getY() const noexcept{
				return _y;
			}

		private:
			float _y;
	};
}