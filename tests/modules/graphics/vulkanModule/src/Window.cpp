#include <Raindrop.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include "Window.hpp"

Window::Window() : Raindrop::Graphics::Window(){
	RAINDROP_profile_function();
	SDL_LogSetOutputFunction(&SDL_log, nullptr);

	window = SDL_CreateWindow("window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, SDL_WINDOW_VULKAN);
	if (window == nullptr){
		RAINDROP_log(ERR, API, "failed to create an SDL2 window : %s", SDL_GetError());
		return;
	}
}

Window::~Window(){
	RAINDROP_profile_function();
	SDL_DestroyWindow(window);
}

void Window::setTitle(const char* title){
	RAINDROP_profile_function();
	SDL_SetWindowTitle(window, title);
}

void Window::setSize(Raindrop::Core::Maths::vec2<Raindrop::uint32> size){
	RAINDROP_profile_function();
	SDL_SetWindowSize(window, (int)size.x, (int)size.y);
}

void Window::setPosition(Raindrop::Core::Maths::vec2<Raindrop::uint32> position){
	RAINDROP_profile_function();
	SDL_SetWindowPosition(window, (int)position.x, (int)position.y);
}

const char* Window::getTitle() const{
	return SDL_GetWindowTitle(window);
}

Raindrop::Core::Maths::vec2<Raindrop::uint32> Window::getSize() const{
	RAINDROP_profile_function();
	Raindrop::Core::Maths::vec2<int> size;
	SDL_GetWindowSize(window, &size.x, &size.y);
	return Raindrop::Core::Maths::vec2<Raindrop::uint32>(size);	
}

Raindrop::Core::Maths::vec2<Raindrop::uint32> Window::getPosition() const{
	RAINDROP_profile_function();
	Raindrop::Core::Maths::vec2<int> position;
	SDL_GetWindowPosition(window, &position.x, &position.y);
	return Raindrop::Core::Maths::vec2<Raindrop::uint32>(position);	
}

const char* Window::getAPI() const{
	RAINDROP_profile_function();
	return "SDL2";
}

bool Window::loaded() const{
	RAINDROP_profile_function();
	return window != nullptr;
}

Raindrop::Core::Debug::Log::Severity Window::SDL_to_raindrop_severity(SDL_LogPriority priority){
	RAINDROP_profile_function();
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
	RAINDROP_profile_function();
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
	RAINDROP_profile_function();
	return window;
}

VkSurfaceKHR Window::createSurface(VkInstance instance){
	VkSurfaceKHR surface;
	SDL_Vulkan_CreateSurface(window, instance, &surface);
	return surface;
}

extern "C"{
	Raindrop::Graphics::Window* RAINDROP_MODULE createWindow(Raindrop::Core::Memory::Allocator& allocator){
		RAINDROP_profile_function();
		return Raindrop::Core::Memory::allocateNew<Window>(allocator);
	}
}