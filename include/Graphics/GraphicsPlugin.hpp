#ifndef __RAINDROP_GRAPHICS_PLUGIN_HPP__
#define __RAINDROP_GRAPHICS_PLUGIN_HPP__

#include "Core/Memory/allocators/Allocator.hpp"
#include "Context.hpp"
#include "Renderer.hpp"
#include "Core/Scene/Scene.hpp"

namespace Raindrop::Graphics{
	class Plugin{
		public:
			Plugin(Context& context, Core::Scene::Scene& scene){};
			virtual ~Plugin() = default;

			virtual const char* getName() const = 0;
			virtual version getVersion() const = 0;

			static Plugin* create(Core::Memory::Allocator& allocator, Core::IO::Module& module, Context& context, Core::Scene::Scene& scene, const char* name);
		
		protected:
			Plugin() = default;
	};
}

#endif