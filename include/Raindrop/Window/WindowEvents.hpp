#pragma once

#include "Raindrop/Event/Event.hpp"
#include <glm/glm.hpp>

namespace Raindrop::Window{

	/**
	 * @brief Base window event class
	 * 
	 */
	class WindowEvent : public Event::Event{
		public:
			WindowEvent() : Event(){}
			~WindowEvent() = default;
	};

	/**
	 * @brief Triggerd after the window has been resized
	 * 
	 */
	class WindowResized : public WindowEvent{
		public:
			WindowResized(const glm::u32vec2& size) : _size{size}{}
			~WindowResized() = default;

			const glm::u32vec2& getSize() const noexcept{
				return _size;
			}

		protected:
			glm::u32vec2 _size;
	};

	/**
	 * @brief Triggers after the window has been shown
	 * 
	 */
	class WindowShown : public WindowEvent{
		public:
			WindowShown(){}
			~WindowShown() = default;
	};

	/**
	 * @brief Triggers after the window has been hidden
	 * 
	 */
	class WindowHidden : public WindowEvent{
		public:
			WindowHidden(){}
			~WindowHidden() = default;
	};

	/**
	 * @brief Triggers after the window has been exposed
	 * 
	 */
	class WindowExposed : public WindowEvent{
		public:
			WindowExposed(){}
			~WindowExposed() = default;
	};

	/**
	 * @brief Triggers after the window has been exposed
	 * 
	 */
	class WindowMoved : public WindowEvent{
		public:
			WindowMoved(const glm::ivec2& position) : _position{position}{}
			~WindowMoved() = default;

			const glm::ivec2& getPosition() const noexcept{
				return _position;
			}
		
		private:
			glm::ivec2 _position;
	};

	/**
	 * @brief Triggers after the window has been minimized
	 * 
	 */
	class WindowMinimized : public WindowEvent{
		public:
			WindowMinimized(){}
			~WindowMinimized() = default;
	};

	/**
	 * @brief Triggers after the window has been maximized
	 * 
	 */
	class WindowMaximized : public WindowEvent{
		public:
			WindowMaximized(){}
			~WindowMaximized() = default;
	};

	/**
	 * @brief Triggers after the mouse has entered the window
	 * 
	 */
	class WindowMouseEntered : public WindowEvent{
		public:
			WindowMouseEntered(){}
			~WindowMouseEntered() = default;
	};

	/**
	 * @brief Triggers after the mouse has leaved the window
	 * 
	 */
	class WindowMouseLeaved : public WindowEvent{
		public:
			WindowMouseLeaved(){}
			~WindowMouseLeaved() = default;
	};

	/**
	 * @brief Triggers after the window has been restored
	 * 
	 */
	class WindowRestored : public WindowEvent{
		public:
			WindowRestored(){}
			~WindowRestored() = default;
	};

	/**
	 * @brief Triggers after the window has gained focus
	 * 
	 */
	class WindowFocusGained : public WindowEvent{
		public:
			WindowFocusGained(){}
			~WindowFocusGained() = default;
	};

	/**
	 * @brief Triggers after the window has lost focus
	 * 
	 */
	class WindowFocusLost : public WindowEvent{
		public:
			WindowFocusLost(){}
			~WindowFocusLost() = default;
	};

	/**
	 * @brief Triggers after the window has been requested to be closed
	 * 
	 */
	class WindowCloseRequest : public WindowEvent{
		public:
			WindowCloseRequest(){}
			~WindowCloseRequest() = default;
	};

	/**
	 * @brief Triggers after the window took the focus
	 * 
	 */
	class WindowFocusTake : public WindowEvent{
		public:
			WindowFocusTake(){}
			~WindowFocusTake() = default;
	};

	/**
	 * @brief Triggers after the window has been occluded
	 * 
	 */
	class WindowOccluded : public WindowEvent{
		public:
			WindowOccluded(){}
			~WindowOccluded() = default;
	};

	/**
	 * @brief Triggers after the window leaves fullscreen mode
	 * 
	 */
	class WindowFullscreenLeave : public WindowEvent{
		public:
			WindowFullscreenLeave(){}
			~WindowFullscreenLeave() = default;
	};

	/**
	 * @brief Triggers after the window enters fullscreen mode
	 * 
	 */
	class WindowFullscreenEnter : public WindowEvent{
		public:
			WindowFullscreenEnter(){}
			~WindowFullscreenEnter() = default;
	};

	/**
	 * @brief Triggers after a window has been destroyed
	 * 
	 */
	class WindowDestroyed : public Event::Event{
		public:
			WindowDestroyed() : Event(){}
			~WindowDestroyed() = default;
	};
}