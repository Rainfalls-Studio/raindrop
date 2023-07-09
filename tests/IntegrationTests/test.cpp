#include <Raindrop/Core/Engine.hpp>

static std::filesystem::path RESOURCES = RESOURCES_PATH;

int main(int argc, char **argv){
	START_EASYLOGGINGPP(argc, argv);
	Raindrop::Core::Engine engine;

	engine.registry()["Engine.Resources.Directory"] = RESOURCES.string();
	auto pipeline = engine.assetManager().loadOrGet(RESOURCES / "pipelines/default.xml.gfxpipe");
	engine.assetManager().loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/models/teapot.obj");

	// /!\ TEST
	engine.registry()["Pipeline"] = pipeline;
	// /!\ TEST

	// auto entity = engine.scene().createEntity();
	engine.renderer().openGUI(RESOURCES / "gui/gui.xml");
	engine.mainloop();

	return 0;
}