#include "Renderer.hpp"
#include "Core/Debug/logger.hpp"
#include "Core/Debug/profiler.hpp"

namespace Raindrop::Graphics{
	Renderer::Renderer(){}
	
	Renderer* Renderer::create(Core::Memory::Allocator& allocator, Core::IO::Module& module, const char* fncName){
		using CreateRendererFnc = Renderer*(*)(Core::Memory::Allocator&);
		CreateRendererFnc createRenderer = (CreateRendererFnc)module.getFnc(fncName);
		
		if (!createRenderer){
			RAINDROP_log(ERROR,GRAPHICS, "failed to found \"%s\" function for creating a renderer in \"%s\" module", fncName, module.filepath());
			return nullptr;
		}
		
		return createRenderer(allocator);
	}
}