#include <Raindrop.hpp>
#include <SDL2/SDL.h>
#include "Window.hpp"

static Raindrop::uint32 windowCount = 0;

Window::Window() : Raindrop::Graphics::Window(){
	SDL_LogSetOutputFunction(&SDL_log, nullptr);

	if (windowCount == 0){
		if (SDL_Init(SDL_INIT_VIDEO) != 0){
			RAINDROP_log(ERR, API, "failed to initialize the SDL2 library : %s", SDL_GetError());
			return;
		}
		windowCount++;
	}

	window = SDL_CreateWindow("window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, SDL_WINDOW_OPENGL);
	if (window == nullptr){
		RAINDROP_log(ERR, API, "failed to create an SDL2 window : %s", SDL_GetError());
		return;
	}
}

Window::~Window(){
	SDL_DestroyWindow(window);
	windowCount--;

	if (windowCount == 0){
		SDL_Quit();
	}
}

void Window::setTitle(const char* title){
	SDL_SetWindowTitle(window, title);
}

void Window::setSize(Raindrop::Core::Maths::vec2<Raindrop::uint32> size){
	SDL_SetWindowSize(window, (int)size.x, (int)size.y);
}

void Window::setPosition(Raindrop::Core::Maths::vec2<Raindrop::uint32> position){
	SDL_SetWindowPosition(window, (int)position.x, (int)position.y);
}

const char* Window::getTitle() const{
	return SDL_GetWindowTitle(window);
}

Raindrop::Core::Maths::vec2<Raindrop::uint32> Window::getSize() const{
	Raindrop::Core::Maths::vec2<int> size;
	SDL_GetWindowSize(window, &size.x, &size.y);
	return Raindrop::Core::Maths::vec2<Raindrop::uint32>(size);	
}

Raindrop::Core::Maths::vec2<Raindrop::uint32> Window::getPosition() const{
	Raindrop::Core::Maths::vec2<int> position;
	SDL_GetWindowPosition(window, &position.x, &position.y);
	return Raindrop::Core::Maths::vec2<Raindrop::uint32>(position);	
}

const char* Window::getAPI() const{
	return "SDL2";
}

bool Window::loaded() const{
	return window != nullptr;
}

Raindrop::Core::Debug::Log::Severity Window::SDL_to_raindrop_severity(SDL_LogPriority priority){
	switch (priority){
		case SDL_LOG_PRIORITY_VERBOSE: return Raindrop::Core::Debug::Log::Severity::TRACE;
		case SDL_LOG_PRIORITY_DEBUG: return Raindrop::Core::Debug::Log::Severity::DEBUG;
		case SDL_LOG_PRIORITY_INFO: return Raindrop::Core::Debug::Log::Severity::INFO;
		case SDL_LOG_PRIORITY_WARN: return Raindrop::Core::Debug::Log::Severity::WARNING;
		case SDL_LOG_PRIORITY_ERROR: return Raindrop::Core::Debug::Log::Severity::ERROR;
		case SDL_LOG_PRIORITY_CRITICAL: return Raindrop::Core::Debug::Log::Severity::CRITICAL;
	}
	return Raindrop::Core::Debug::Log::Severity::WARNING;
}

void Window::SDL_log(void* userData, int category, SDL_LogPriority priority, const char* message){
	Raindrop::Core::Debug::Log::Log log;

	log.cause = Raindrop::Core::Debug::Log::Cause::API;
	log.condition = "none";
	log.file = __FILE__;
	log.function = __PRETTY_FUNCTION__;
	strcpy(log.message, message);
	log.severity = SDL_to_raindrop_severity(priority);

	Raindrop::Core::Debug::log(log);
}

SDL_Window* Window::getWindow() const{
	return window;
}

extern "C"{
	Raindrop::Graphics::Window* RAINDROP_MODULE createWindow(Raindrop::Core::Memory::Allocator& allocator){
		return Raindrop::Core::Memory::allocateNew<Window>(allocator);
	}
}