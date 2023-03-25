#ifndef __RAINDROP_GRAPHICS_PLUGIN_HPP__
#define __RAINDROP_GRAPHICS_PLUGIN_HPP__

#include "Core/Memory/allocators/Allocator.hpp"
#include "Context.hpp"
#include "Renderer.hpp"

namespace Raindrop::Graphics{
	class Plugin{
		public:
			Plugin(Context& context) : _context{context}{}
			virtual ~Plugin() = default;

			virtual const char* getName() const = 0;
			virtual version getVersion() const = 0;

			static Plugin* create(Core::Memory::Allocator& allocator, Core::IO::Module& module, Context& context, const char* name);
		
		protected:
			Context& _context;
	};
}

#endif