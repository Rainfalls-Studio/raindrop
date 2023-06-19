#include <Raindrop/Graphics/Window.hpp>
#include <Raindrop/Graphics/Instance.hpp>
#include <Raindrop/Graphics/PhysicalDeviceManager.hpp>
#include <Raindrop/Graphics/PhysicalDevice.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/Swapchain.hpp>
#include <Raindrop/Graphics/Renderer.hpp>
#include <Raindrop/Graphics/ImGUI.hpp>

#include <SDL2/SDL_vulkan.h>

namespace Raindrop::Graphics{

	static constexpr uint32_t DEFAULT_WINDOW_WIDTH = 1080;
	static constexpr uint32_t DEFAULT_WINDOW_HEIGHT = 720;

	Window::Window(Core::Event::EventManager& eventManager, Core::Registry::Registry& registry) : _eventManager{eventManager}, _registry{registry}{
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

	void Window::events(ImGUI* gui){
		SDL_Event e;
		while (SDL_PollEvent(&e)){
			if (gui) gui->event(&e);
			switch (e.type){
				case SDL_QUIT: quitEvent(); break;
				case SDL_WINDOWEVENT: windowEvent(e); break;
				case SDL_MOUSEMOTION: mouseMotionEvent(e); break;
				case SDL_KEYDOWN: keyDown(e); break;
				case SDL_KEYUP: keyUp(e); break;
				case SDL_MOUSEBUTTONDOWN: mouseDown(e); break;
				case SDL_MOUSEBUTTONUP: mouseUp(e); break;
			}
		}
	}

	void Window::windowEvent(SDL_Event& e){
		switch (e.window.event){
			case SDL_WINDOWEVENT_RESIZED: windowResizedEvent(e.window); break;
		}
	}

	void Window::quitEvent(){
		_eventManager.trigger("Engine.Window.Quit");
	}

	void Window::windowResizedEvent(SDL_WindowEvent& e){
		_resized = true;
		_registry["Engine.Window.Size"] = glm::vec2((float)e.data1, (float)e.data2);
		_eventManager.trigger("Engine.Window.Resized");
	}

	void Window::mouseMotionEvent(SDL_Event& e){
		_eventManager.trigger("Engine.Mouse.Mouved");
		_eventManager.mouseEvents().pos() = glm::vec2(e.motion.x, e.motion.y);
		_eventManager.mouseEvents().relPos() = glm::vec2(e.motion.xrel, e.motion.yrel);
	}

	Core::Event::MouseButton SDLToRaindropMouseButton(Uint8 button){
		switch (button){
			case SDL_BUTTON_LEFT: return Core::Event::BUTTON_LEFT;
			case SDL_BUTTON_MIDDLE: return Core::Event::BUTTON_MIDDLE;
			case SDL_BUTTON_RIGHT: return Core::Event::BUTTON_RIGHT;
		}
		throw std::runtime_error("unknow button");
	}

	void Window::mouseDown(SDL_Event& e){
		_eventManager.trigger("Engine.Mouse.ButtonDown");
		_eventManager.mouseEvents().state(SDLToRaindropMouseButton(e.button.button)) = Core::Event::BUTTON_DOWN;
	}

	void Window::mouseUp(SDL_Event& e){
		_eventManager.trigger("Engine.Mouse.ButtonUp");
		_eventManager.mouseEvents().state(SDLToRaindropMouseButton(e.button.button)) = Core::Event::BUTTON_UP;
	}
	
	bool Window::resized() const{
		return _resized;
	}

	void Window::resetResizedFlag(){
		_resized = false;
	}

	void Window::keyDown(SDL_Event& e){
		_registry["Engine.Key.Down"] = SDL_GetScancodeName(e.key.keysym.scancode);
		_eventManager.trigger("Engine.KeyDown");
		_eventManager.keyEvents()[static_cast<Core::Event::Key>(e.key.keysym.scancode)] = Core::Event::KEY_PRESSED;
	}

	void Window::keyUp(SDL_Event& e){
		_registry["Engine.Key.Up"] = SDL_GetScancodeName(e.key.keysym.scancode);
		_eventManager.trigger("Engine.KeyUp");
		_eventManager.keyEvents()[static_cast<Core::Event::Key>(e.key.keysym.scancode)] = Core::Event::KEY_RELEASED;
	}
}