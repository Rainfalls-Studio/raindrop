#pragma once

#include "Raindrop/pch.pch"
#include "Raindrop/Core/Event/Event.hpp"
#include "../Window/Window.hpp"

namespace Raindrop::Platform::Event{

	/**
	 * @brief Base window event class
	 * 
	 */
	class WindowEvent : public Core::Internal::Event::Event{
		public:

			WindowEvent(Window::Window window) : Event(), _window{window}{};
			~WindowEvent() = default;
			
			inline const Window::Window getWindow() const noexcept{
				return _window;
			}

		protected:
			Window::Window _window;
	};

	/**
	 * @brief Triggerd after the window has been resized
	 * 
	 */
	class WindowResized : public WindowEvent{
		public:
			WindowResized(Window::Window window, const glm::u32vec2& size) : WindowEvent(window), _size{size}{}
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
			WindowShown(Window::Window window) : WindowEvent(window){}
			~WindowShown() = default;
	};

	/**
	 * @brief Triggers after the window has been hidden
	 * 
	 */
	class WindowHidden : public WindowEvent{
		public:
			WindowHidden(Window::Window window) : WindowEvent(window){}
			~WindowHidden() = default;
	};

	/**
	 * @brief Triggers after the window has been exposed
	 * 
	 */
	class WindowExposed : public WindowEvent{
		public:
			WindowExposed(Window::Window window) : WindowEvent(window){}
			~WindowExposed() = default;
	};

	/**
	 * @brief Triggers after the window has been exposed
	 * 
	 */
	class WindowMoved : public WindowEvent{
		public:
			WindowMoved(Window::Window window, const glm::u32vec2& position) : WindowEvent(window), _position{position}{}
			~WindowMoved() = default;

			const glm::u32vec2& getPosition() const noexcept{
				return _position;
			}
		
		private:
			glm::u32vec2 _position;
	};

	/**
	 * @brief Triggers after the window has been minimized
	 * 
	 */
	class WindowMinimized : public WindowEvent{
		public:
			WindowMinimized(Window::Window window) : WindowEvent(window){}
			~WindowMinimized() = default;
	};

	/**
	 * @brief Triggers after the window has been maximized
	 * 
	 */
	class WindowMaximized : public WindowEvent{
		public:
			WindowMaximized(Window::Window window) : WindowEvent(window){}
			~WindowMaximized() = default;
	};

	/**
	 * @brief Triggers after the mouse has entered the window
	 * 
	 */
	class WindowMouseEntered : public WindowEvent{
		public:
			WindowMouseEntered(Window::Window window) : WindowEvent(window){}
			~WindowMouseEntered() = default;
	};

	/**
	 * @brief Triggers after the mouse has leaved the window
	 * 
	 */
	class WindowMouseLeaved : public WindowEvent{
		public:
			WindowMouseLeaved(Window::Window window) : WindowEvent(window){}
			~WindowMouseLeaved() = default;
	};

	/**
	 * @brief Triggers after the window has been restored
	 * 
	 */
	class WindowRestored : public WindowEvent{
		public:
			WindowRestored(Window::Window window) : WindowEvent(window){}
			~WindowRestored() = default;
	};

	/**
	 * @brief Triggers after the window has gained focus
	 * 
	 */
	class WindowFocusGained : public WindowEvent{
		public:
			WindowFocusGained(Window::Window window) : WindowEvent(window){}
			~WindowFocusGained() = default;
	};

	/**
	 * @brief Triggers after the window has lost focus
	 * 
	 */
	class WindowFocusLost : public WindowEvent{
		public:
			WindowFocusLost(Window::Window window) : WindowEvent(window){}
			~WindowFocusLost() = default;
	};

	/**
	 * @brief Triggers after the window has been requested to be closed
	 * 
	 */
	class WindowCloseRequest : public WindowEvent{
		public:
			WindowCloseRequest(Window::Window window) : WindowEvent(window){}
			~WindowCloseRequest() = default;
	};

	/**
	 * @brief Triggers after the window took the focus
	 * 
	 */
	class WindowFocusTake : public WindowEvent{
		public:
			WindowFocusTake(Window::Window window) : WindowEvent(window){}
			~WindowFocusTake() = default;
	};

	/**
	 * @brief Triggers after the window has been occluded
	 * 
	 */
	class WindowOccluded : public WindowEvent{
		public:
			WindowOccluded(Window::Window window) : WindowEvent(window){}
			~WindowOccluded() = default;
	};

	/**
	 * @brief Triggers after the window leaves fullscreen mode
	 * 
	 */
	class WindowFullscreenLeave : public WindowEvent{
		public:
			WindowFullscreenLeave(Window::Window window) : WindowEvent(window){}
			~WindowFullscreenLeave() = default;
	};

	/**
	 * @brief Triggers after the window enters fullscreen mode
	 * 
	 */
	class WindowFullscreenEnter : public WindowEvent{
		public:
			WindowFullscreenEnter(Window::Window window) : WindowEvent(window){}
			~WindowFullscreenEnter() = default;
	};

	/**
	 * @brief Triggers after a window has been destroyed
	 * 
	 */
	class WindowDestroyed : public Core::Internal::Event::Event{
		public:
			WindowDestroyed() : Event(){}
			~WindowDestroyed() = default;
	};
}