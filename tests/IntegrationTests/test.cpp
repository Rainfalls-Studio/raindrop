#include <Raindrop/Core/Engine.hpp>

static std::filesystem::path RESOURCES = RESOURCES_PATH;

int main(int argc, char **argv){
	START_EASYLOGGINGPP(argc, argv);
	Raindrop::Core::Engine engine;
	
	engine.assetManager().loadOrGet(RESOURCES / "shaders/default/default.glsl.vert.spv");
	engine.assetManager().loadOrGet(RESOURCES / "shaders/default/default.glsl.frag.spv");

	engine.mainloop();
	return 0;
	return 0;
}