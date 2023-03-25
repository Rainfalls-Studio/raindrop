#include "Context.hpp"
#include "Renderer.hpp"
#include "Core/Debug/logger.hpp"
#include "Core/Debug/profiler.hpp"
#include "Renderer.hpp"
#include "GraphicsPlugin.hpp"

namespace Raindrop::Graphics{
	Context::Context(){}
	
	Context* Context::create(Core::Memory::Allocator& allocator, Renderer& renderer, Window& window){
		RAINDROP_profile_function();
		return renderer.createContext(allocator, window);
	}

	Plugin* Context::loadPlugin(Core::Memory::Allocator& allocator, Core::IO::Module& module, const char* name){
		RAINDROP_profile_function();
		return Plugin::create(allocator, module, *this, name);
	}

}