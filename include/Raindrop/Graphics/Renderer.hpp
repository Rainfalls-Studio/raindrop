#ifndef __RAINDROP_GRAPHICS_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_RENDERER_HPP__

#include <common.hpp>
#include <Core/IO/Module.hpp>
#include <Core/Memory/allocators/Allocator.hpp>
#include <Core/Maths/Maths.hpp>

namespace Raindrop::Graphics{
	class Context;
	class Window;

	class RAINDROP_API Renderer{
		public:
			Renderer();
			virtual ~Renderer() = default;

			virtual Context* createContext(Core::Memory::Allocator& allocator, Window& window) = 0;
			virtual const char* getAPI() const = 0;
			virtual bool loaded() const = 0;

			static Renderer* create(Core::Memory::Allocator& allocator, Core::IO::Module& module, const char* fnc = "createRenderer");
	};
}

#endif