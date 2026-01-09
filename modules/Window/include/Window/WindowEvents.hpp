#pragma once

#include "Window.hpp"
#include <Event/Event.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace Window::Events{

	/**
	 * @brief Base window event class
	 * 
	 */
	class WindowEvent : public ::Event::Event{
		public:
			inline WindowEvent(const std::shared_ptr<Window>& window) noexcept : Event(), _window{window} {}
			~WindowEvent() = default;

			inline const std::shared_ptr<Window>& getWindow() const{
				return _window;
			}
		
		private:
			std::shared_ptr<Window> _window;
	};

	/**
	 * @brief Triggerd after the window has been resized
	 * 
	 */
	class WindowResized : public WindowEvent{
		public:
			inline WindowResized(const std::shared_ptr<Window>& window, const glm::u32vec2& size) noexcept : WindowEvent(window), _size{size}{}
			~WindowResized() = default;

			inline const glm::u32vec2& getSize() const noexcept{
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
			inline WindowShown(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowShown() = default;
	};

	/**
	 * @brief Triggers after the window has been hidden
	 * 
	 */
	class WindowHidden : public WindowEvent{
		public:
			inline WindowHidden(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowHidden() = default;
	};

	/**
	 * @brief Triggers after the window has been exposed
	 * 
	 */
	class WindowExposed : public WindowEvent{
		public:
			inline WindowExposed(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowExposed() = default;
	};

	/**
	 * @brief Triggers after the window has been exposed
	 * 
	 */
	class WindowMoved : public WindowEvent{
		public:
			inline WindowMoved(const std::shared_ptr<Window>& window, const glm::ivec2& position) noexcept : WindowEvent(window), _position{position}{}
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
			inline WindowMinimized(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowMinimized() = default;
	};

	/**
	 * @brief Triggers after the window has been maximized
	 * 
	 */
	class WindowMaximized : public WindowEvent{
		public:
			inline WindowMaximized(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowMaximized() = default;
	};

	/**
	 * @brief Triggers after the mouse has entered the window
	 * 
	 */
	class WindowMouseEntered : public WindowEvent{
		public:
			inline WindowMouseEntered(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowMouseEntered() = default;
	};

	/**
	 * @brief Triggers after the mouse has leaved the window
	 * 
	 */
	class WindowMouseLeaved : public WindowEvent{
		public:
			inline WindowMouseLeaved(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowMouseLeaved() = default;
	};

	/**
	 * @brief Triggers after the window has been restored
	 * 
	 */
	class WindowRestored : public WindowEvent{
		public:
			inline WindowRestored(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowRestored() = default;
	};

	/**
	 * @brief Triggers after the window has gained focus
	 * 
	 */
	class WindowFocusGained : public WindowEvent{
		public:
			inline WindowFocusGained(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowFocusGained() = default;
	};

	/**
	 * @brief Triggers after the window has lost focus
	 * 
	 */
	class WindowFocusLost : public WindowEvent{
		public:
			inline WindowFocusLost(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowFocusLost() = default;
	};

	/**
	 * @brief Triggers after the window has been requested to be closed
	 * 
	 */
	class WindowCloseRequest : public WindowEvent{
		public:
			inline WindowCloseRequest(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowCloseRequest() = default;
	};

	/**
	 * @brief Triggers after the window took the focus
	 * 
	 */
	class WindowFocusTake : public WindowEvent{
		public:
			inline WindowFocusTake(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowFocusTake() = default;
	};

	/**
	 * @brief Triggers after the window has been occluded
	 * 
	 */
	class WindowOccluded : public WindowEvent{
		public:
			inline WindowOccluded(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowOccluded() = default;
	};

	/**
	 * @brief Triggers after the window leaves fullscreen mode
	 * 
	 */
	class WindowFullscreenLeave : public WindowEvent{
		public:
			inline WindowFullscreenLeave(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowFullscreenLeave() = default;
	};

	/**
	 * @brief Triggers after the window enters fullscreen mode
	 * 
	 */
	class WindowFullscreenEnter : public WindowEvent{
		public:
			inline WindowFullscreenEnter(const std::shared_ptr<Window>& window) noexcept : WindowEvent(window){}
			~WindowFullscreenEnter() = default;
	};

	/**
	 * @brief Triggers after a window has been destroyed
	 * 
	 */
	class WindowDestroyed : public ::Event::Event{
		public:
			inline WindowDestroyed() noexcept : Event(){}
			~WindowDestroyed() = default;
	};
}