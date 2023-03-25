#include "Context.hpp"
#include <SDL2/SDL.h>
#include <glad/glad.h>

Context::Context(Renderer& renderer, Window& window) : Raindrop::Graphics::Context(), _renderer(renderer), _window(window){}

Context::~Context(){}

void Context::setContext(SDL_GLContext context){
	_context = context;
}

void Context::swap(){
	SDL_GL_SwapWindow(_window.getWindow());
}

void Context::setClearColor(Raindrop::Core::Maths::vec4<Raindrop::float32> color){
	glClearColor(color.x, color.y, color.z, color.w);
}

void Context::enableVSync(bool enable){
	if (SDL_GL_SetSwapInterval(enable ? 1 : 0) != 0){
		RAINDROP_log(ERROR, GRAPHICS, "failed to set the vertical synchronization to %s : %s", enable ? "true" : "false", SDL_GetError());
	}
}

bool Context::isVSyncEnabled() const{
	return SDL_GL_GetSwapInterval() == 1;
}

extern "C"{
	Raindrop::Graphics::Context* RAINDROP_MODULE createContext(Raindrop::Core::Memory::Allocator& allocator, Raindrop::Graphics::Renderer& renderer, Raindrop::Graphics::Window& window){
		return Raindrop::Core::Memory::allocateNew<Context>(allocator, (Renderer&)renderer, (Window&)window);
	}
}