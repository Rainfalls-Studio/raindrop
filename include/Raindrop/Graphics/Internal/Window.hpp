#ifndef __RAINDROP_GRAPHICS_INTERNAL_WINDOW_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_WINDOW_HPP__

#include <SDL.h>
#include <Raindrop/Graphics/Internal/common.hpp>

namespace Raindrop::Graphics::Internal{
	class Window{
		public:
			Window(Context& context);
			virtual ~Window();
			
			void createSurface();
			void destroySurface();

			void setTitle(const char* title);
			void setSize(glm::u32vec2 size);
			void setPosition(glm::u32vec2 position);
			const char* getTitle() const;
			glm::u32vec2 getSize() const;
			glm::u32vec2 getPosition() const;
			bool loaded() const;
			SDL_Window* get() const;

			void events();
			
			bool resized() const;
			void resetResizedFlag();

			VkSurfaceKHR surface();
			std::vector<const char*> vulkanExtensions();
		
		private:
			Context& _context;
			SDL_Window* _window;
			VkSurfaceKHR _surface;

			void quitEvent();
			void windowEvent(SDL_Event& e);
			void windowResizedEvent(SDL_WindowEvent& e);
			void mouseMotionEvent(SDL_Event& e);
			void mouseDown(SDL_Event& e);
			void mouseUp(SDL_Event& e);
			void keyDown(SDL_Event& e);
			void keyUp(SDL_Event& e);

			bool _resized = false;
	};
}

#endif