#include <Raindrop/Graphics/Window.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/ImGUI.hpp>

#include <SDL2/SDL_vulkan.h>

namespace Raindrop::Graphics{

	static constexpr uint32_t DEFAULT_WINDOW_WIDTH = 1080;
	static constexpr uint32_t DEFAULT_WINDOW_HEIGHT = 720;

	Window::Window(GraphicsContext& context) : _context{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Window");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		if (SDL_Init(SDL_INIT_VIDEO) != 0){
			CLOG(ERROR, "Engine.Graphics.Window") << "Failed to initialize SDL2 API : " << SDL_GetError();
			throw std::runtime_error("Failed initialize SDL2 API");
		}

		_window = SDL_CreateWindow("Raindrop::Graphics window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_VULKAN);

		if (!_window){
			std::stringstream err;
			err << "Failed to create SDL2 window : " << SDL_GetError();
			throw std::runtime_error(err.str());
		}

		_context.gRegistry["Window"] = this;
	}

	Window::~Window(){
		if (_window) SDL_DestroyWindow(_window);

		SDL_Quit();
	}

	std::vector<const char*> Window::vulkanExtensions(){
		uint32_t extensionCount = 0;
		if (!SDL_Vulkan_GetInstanceExtensions(_window, &extensionCount, nullptr)){
			throw std::runtime_error("failed to querry SDL2 vulkan instance extensions");
		}

		std::vector<const char*> extensions(extensionCount);
		SDL_Vulkan_GetInstanceExtensions(_window, &extensionCount, extensions.data());
		return extensions;
	}

	void Window::createSurface(){
		if (SDL_Vulkan_CreateSurface(_window, _context.instance.get(), &_surface) == SDL_FALSE){
			CLOG(ERROR, "Engine.Graphics.Window") << "Failed to create SDL2 Vulkan surface : " << SDL_GetError();
			throw std::runtime_error("Failed to create SDL2 Vulkan surface");
		}
	}

	void Window::destroySurface(){
		if (_surface) vkDestroySurfaceKHR(_context.instance.get(), _surface, _context.allocationCallbacks);
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
		_context->eventManager.trigger("Engine.Window.Quit");
	}

	void Window::windowResizedEvent(SDL_WindowEvent& e){
		_resized = true;
		_context->registry["Engine.Window.Size"] = glm::vec2((float)e.data1, (float)e.data2);
		_context->eventManager.trigger("Engine.Window.Resized");
	}

	void Window::mouseMotionEvent(SDL_Event& e){
		_context->eventManager.trigger("Engine.Mouse.Mouved");
		_context->eventManager.mouseEvents().pos() = glm::vec2(e.motion.x, e.motion.y);
		_context->eventManager.mouseEvents().relPos() = glm::vec2(e.motion.xrel, e.motion.yrel);
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
		_context->eventManager.trigger("Engine.Mouse.ButtonDown");
		_context->eventManager.mouseEvents().state(SDLToRaindropMouseButton(e.button.button)) = Core::Event::BUTTON_DOWN;
	}

	void Window::mouseUp(SDL_Event& e){
		_context->eventManager.trigger("Engine.Mouse.ButtonUp");
		_context->eventManager.mouseEvents().state(SDLToRaindropMouseButton(e.button.button)) = Core::Event::BUTTON_UP;
	}
	
	bool Window::resized() const{
		return _resized;
	}

	void Window::resetResizedFlag(){
		_resized = false;
	}

	void Window::keyDown(SDL_Event& e){
		_context->registry["Engine.Key.Down"] = SDL_GetScancodeName(e.key.keysym.scancode);
		_context->eventManager.trigger("Engine.KeyDown");
		_context->eventManager.keyEvents()[static_cast<Core::Event::Key>(e.key.keysym.scancode)] = Core::Event::KEY_PRESSED;
	}

	void Window::keyUp(SDL_Event& e){
		_context->registry["Engine.Key.Up"] = SDL_GetScancodeName(e.key.keysym.scancode);
		_context->eventManager.trigger("Engine.KeyUp");
		_context->eventManager.keyEvents()[static_cast<Core::Event::Key>(e.key.keysym.scancode)] = Core::Event::KEY_RELEASED;
	}

	VkSurfaceKHR Window::surface(){
		return _surface;
	}
}