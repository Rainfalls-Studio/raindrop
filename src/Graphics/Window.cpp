#include <Raindrop/Graphics/Window.hpp>
#include <Raindrop/Graphics/Instance.hpp>
#include <Raindrop/Graphics/PhysicalDeviceManager.hpp>
#include <Raindrop/Graphics/PhysicalDevice.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/Swapchain.hpp>
#include <Raindrop/Graphics/Renderer.hpp>

#include <SDL2/SDL_vulkan.h>

namespace Raindrop::Graphics{

	static constexpr uint32_t DEFAULT_WINDOW_WIDTH = 1080;
	static constexpr uint32_t DEFAULT_WINDOW_HEIGHT = 720;

	Window::Window(Core::Event::EventManager& eventManager) : _eventManager{eventManager}{
		_window = SDL_CreateWindow("Raindrop::Graphics window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_VULKAN);

		if (!_window){
			std::stringstream err;
			err << "Failed to create SDL2 window : " << SDL_GetError();
			throw std::runtime_error(err.str());
		}
	}

	Window::~Window(){
		if (_window) SDL_DestroyWindow(_window);
	}

	void Window::setTitle(const char* title){
		SDL_SetWindowTitle(_window, title);
	}

	void Window::setSize(glm::u32vec2 size){
		SDL_SetWindowSize(_window, static_cast<int>(size.x), static_cast<int>(size.y));
	}

	void Window::setPosition(glm::u32vec2 position){
		SDL_SetWindowPosition(_window, static_cast<int>(position.x), static_cast<int>(position.y));
	}

	const char* Window::getTitle() const{
		return SDL_GetWindowTitle(_window);
	}

	glm::u32vec2 Window::getSize() const{
		int w, h;
		SDL_GetWindowSize(_window, &w, &h);
		return glm::u32vec2(w, h);
	}

	glm::u32vec2 Window::getPosition() const{
		int x, y;
		SDL_GetWindowPosition(_window, &x, &y);
		return glm::u32vec2(x, y);
	}

	SDL_Window* Window::get() const{
		return _window;
	}

	bool Window::loaded() const{
		return _window != nullptr;
	}

	void Window::events(){
		SDL_Event e;
		while (SDL_PollEvent(&e)){
			switch (e.type){
				case SDL_QUIT: quitEvent(); break;
				case SDL_WINDOWEVENT: windowEvent(e); break;
				case SDL_MOUSEMOTION: mouseMotionEvent(e); break;
			}
		}
	}

	void Window::windowEvent(SDL_Event& e){
		switch (e.window.event){
			case SDL_WINDOWEVENT_RESIZED: windowResizedEvent(e.window); break;
		}
	}

	void Window::quitEvent(){
		_eventManager.trigger("engine.window.quit");
	}

	void Window::windowResizedEvent(SDL_WindowEvent& e){
		_resized = true;
		_eventManager.trigger("engine.window.resized");
	}

	void Window::mouseMotionEvent(SDL_Event& e){
		_eventManager.trigger("engine.mouse.mouved");
	}

	void Window::mouseClicked(SDL_Event& e){
		_eventManager.trigger("engine.mouse.cliked");
	}
	
	bool Window::resized() const{
		return _resized;
	}

	void Window::resetResizedFlag(){
		_resized = false;
	}
}