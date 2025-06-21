#pragma once


namespace Raindrop::Platform::Window{
	class Window{
		public:
			using State = Internal::Window::Window::State;
			using Flags = Internal::Window::Window::Flags;

			Window();
			Window(std::shared_ptr<Internal::Window::Window> impl);
			~Window();

			static Window Create(const char* title, Extent extent, Position position);

			inline bool isValid();

			inline Extent getSize() const;
			inline Position getPosition() const;

			inline const char* getTitle() const;
			inline State getState() const;

			inline bool isFullscreen() const;
			inline bool isMinimized() const;
			inline bool isMaximized() const;

			inline bool resized() const;
			inline bool moved() const;
			
			inline void setSize(Extent size);
			inline void setPosition(Position position);

			inline void setState(State state);

			inline void setFullscreen();
			inline void setMinimized();
			inline void setMaximized();

			inline void* getNativeHandle() const;

			inline void setEventManager(Core::Event::Manager manager);
			inline void handleEvents();
			inline const Input::InputStateManager& getInputStateManager() const noexcept;


			template<typename T, typename... Args>
			inline T& addProperty(Args&&... args);

			template<typename T>
			inline T& addProperty();

			template<typename T>
			inline T& getProperty();

			template<typename T>
			inline const T& getProperty() const;

			template<typename T>
			inline bool hasProperty() const noexcept;

			template<typename T>
			inline void removeProperty() noexcept;

			inline std::shared_ptr<Internal::Window::Window> getImpl();


		private:
			std::shared_ptr<Internal::Window::Window> _impl;

	};
}

#include "inl/Window.inl"