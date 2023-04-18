#include <Core/Debug/logger.hpp>
#include <Core/Debug/profiler.hpp>
#include <Graphics/Window.hpp>

namespace Raindrop::Graphics{
	Window::Window(){}
	Window::~Window(){}

	Window* Window::create(Core::Memory::Allocator& allocator, Core::IO::Module& module, const char* fncName){
		using CreateWindowFnc = Window*(*)(Core::Memory::Allocator&);
		CreateWindowFnc createWindow = (CreateWindowFnc)module.getFnc(fncName);
		
		if (!createWindow){
			RAINDROP_log(ERROR, GRAPHICS, "failed to found \"%s\" function for creating a window in \"%s\" module", fncName, module.filepath());
			return nullptr;
		}
		
		return createWindow(allocator);
	}
}