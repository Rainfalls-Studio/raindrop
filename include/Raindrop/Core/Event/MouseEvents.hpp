#ifndef __RAINDROP_CORE_EVENT_MOUSE_EVENTS_HPP__
#define __RAINDROP_CORE_EVENT_MOUSE_EVENTS_HPP__

#include <Raindrop/Core/Event/common.hpp>

namespace Raindrop::Core::Event{
	enum MouseButton{
		BUTTON_LEFT,
		BUTTON_MIDDLE,
		BUTTON_RIGHT,
		BUTTON_COUNT,
	};

	enum ButtonState{
		BUTTON_DOWN,
		BUTTON_UP,
	};

	class MouseEvents{
		public:
			MouseEvents(){
				for (int i=0; i<static_cast<int>(BUTTON_COUNT); i++){
					_states[i] = BUTTON_DOWN;
				}
			}

			~MouseEvents() = default;

			ButtonState& state(MouseButton button){
				return _states[static_cast<int>(button)];
			}

			const ButtonState& state(MouseButton button) const{
				return _states[static_cast<int>(button)];
			}

			glm::vec2& pos(){
				return _pos;
			}

			const glm::vec2& pos() const{
				return _pos;
			}

			glm::vec2& relPos(){
				return _relPos;
			}

			const glm::vec2& relPos() const{
				return _relPos;
			}

			glm::vec2& scroll(){
				return _scroll;
			}

			const glm::vec2& scroll() const{
				return _scroll;
			}

		private:
			ButtonState _states[static_cast<int>(BUTTON_COUNT)];

			glm::vec2 _pos;
			glm::vec2 _relPos;
			glm::vec2 _scroll;
			
	};
}

#endif