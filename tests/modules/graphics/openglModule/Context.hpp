#ifndef __OPENGLMODULE_CONTEXT_HPP__
#define __OPENGLMODULE_CONTEXT_HPP__

#include <Raindrop.hpp>
#include <SDL2/SDL.h>
#include "Renderer.hpp"
#include "Window.hpp"

class Context : public Raindrop::Graphics::Context{
	public:
		Context(Renderer& renderer, Window& window);
		~Context();

		void setContext(SDL_GLContext context);
		virtual void swap() override;
		virtual void setClearColor(Raindrop::Core::Maths::vec4<Raindrop::float32> color) override;
		virtual void enableVSync(bool enable) override;
		virtual bool isVSyncEnabled() const override;

	private:
		SDL_GLContext _context;
		Renderer& _renderer;
		Window& _window;

};

#endif