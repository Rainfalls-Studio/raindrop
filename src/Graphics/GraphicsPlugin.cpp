#include <Graphics/GraphicsPlugin.hpp>

namespace Raindrop::Graphics{
	GraphicsPlugin::GraphicsPlugin(const std::filesystem::path& path) : _module{path}{}

	std::shared_ptr<Renderer> GraphicsPlugin::createRenderer(){
		using CreateRendererFnc = std::shared_ptr<Renderer>(*)(void);
		CreateRendererFnc fnc = (CreateRendererFnc)_module.getFnc("create_renderer");
		return fnc();
	}

	std::shared_ptr<Window> GraphicsPlugin::createWindow(){
		using CreateWindowFnc = std::shared_ptr<Window>(*)(void);
		CreateWindowFnc fnc = (CreateWindowFnc)_module.getFnc("create_window");
		return fnc();
	}
}