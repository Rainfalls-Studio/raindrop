#ifndef __OPENGLMODULE_RENDERER_HPP__
#define __OPENGLMODULE_RENDERER_HPP__

#include <Raindrop.hpp>
#include "Window.hpp"

class Renderer : public Raindrop::Graphics::Renderer{
	public:
		Renderer();
		~Renderer();

		virtual Raindrop::Graphics::Context* createContext(Raindrop::Core::Memory::Allocator& allocator, Raindrop::Graphics::Window& window) override;
		virtual const char* getAPI() const override;
		virtual bool loaded() const override;

	private:
		bool _loaded;
		Raindrop::uint32 contextCount;

		void loadGlad();
};

#endif