#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <Raindrop.hpp>

using namespace Raindrop;
using namespace Core;

#define API __declspec(dllexport) __stdcall

extern "C"{
	SDL_Window* window;

	bool API initialize(){
		if (SDL_Init(SDL_INIT_VIDEO) != 0){
			return false;
		}

		window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN);
		return window != nullptr;
	}

	void API shutdown(){
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void API setTitle(const char* title){
		SDL_SetWindowTitle(window, title);
	}

	void API setSize(Maths::vec2<uint32> size){
		SDL_SetWindowSize(window, (int)size.x, (int)size.y);
	}

	void API setPosition(Maths::vec2<uint32> position){
		SDL_SetWindowPosition(window, (int)position.x, (int)position.y);
	}

	const char* API getTitle(){
		return SDL_GetWindowTitle(window);
	}

	Maths::vec2<uint32> API getSize(){
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		return Maths::vec2<uint32>((uint32)w, (uint32)h);
	}

	Maths::vec2<uint32> API getPosition(){
		int x, y;
		SDL_GetWindowPosition(window, &x, &y);
		return Maths::vec2<uint32>((uint32)x, (uint32)y);
	}

	const char* API getAPI(){
		return "SDL2";
	}
}