#include <Raindrop/Core/Engine.hpp>

static std::filesystem::path RESOURCES = RESOURCES_PATH;

int main(int argc, char **argv){
	START_EASYLOGGINGPP(argc, argv);
	Raindrop::Core::Engine engine;

	engine.registry()["Engine.Resources.Directory"] = RESOURCES.string();
	
	engine.assetManager().loadOrGet(RESOURCES / "pipelines/default.xml.gfxpipe");

	engine.mainloop();
	return 0;
}