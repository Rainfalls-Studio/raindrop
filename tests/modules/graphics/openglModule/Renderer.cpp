#include <Raindrop.hpp>
#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "Renderer.hpp"
#include "Context.hpp"

Renderer::Renderer() : Raindrop::Graphics::Renderer(){
	// SDL_GL_LoadLibrary(nullptr);
	contextCount = 0;
	_loaded = true;
}

Renderer::~Renderer(){
	// SDL_GL_UnloadLibrary();
}

Raindrop::Graphics::Context* Renderer::createContext(Raindrop::Core::Memory::Allocator& allocator, Raindrop::Graphics::Window& __window){
	Window& window = (Window&)__window;

	Context* context = Raindrop::Core::Memory::allocateNew<Context>(allocator, *this, window);

	if (!context){
		RAINDROP_log(ERR, GRAPHICS, "failed to allocate a new opengl context : not enought memory");
		return nullptr;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	SDL_GLContext glContext = SDL_GL_CreateContext(window.getWindow());
	if (!glContext){
		RAINDROP_log(ERROR, GRAPHICS, "failed to create an OpenGl context from a SDL2 window : %s", SDL_GetError());
		return nullptr;
	}

	context->setContext(glContext);

	contextCount++;
	if (contextCount == 1){
		loadGlad();

		if (!_loaded){
			Raindrop::Core::Memory::deallocateDelete(allocator, context);
			return nullptr;
		}
	}
	
	return context;
}

const char* Renderer::getAPI() const{
	return "OpenGl";
}

bool Renderer::loaded() const{
	return _loaded;
}

void Renderer::loadGlad(){
	_loaded = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	if (!_loaded){
		RAINDROP_log(ERR, GRAPHICS, "failed to initalize GLAD (OpenGl)");
		return;
	}

	RAINDROP_log(INFO, GRAPHICS, "GLAD OpenGl loaded with success !");

}

extern "C"{
	Raindrop::Graphics::Renderer* RAINDROP_MODULE createRenderer(Raindrop::Core::Memory::Allocator& allocator){
		return Raindrop::Core::Memory::allocateNew<Renderer>(allocator);
	}
}