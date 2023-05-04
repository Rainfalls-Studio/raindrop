#include <Application.hpp>
#include <Core/Debug/profiler.hpp>

namespace Raindrop{
	RAINDROP_API Application::Application(const char* name, Version version){
		RAINDROP_profile_function();
		_name = name;
		_version = version;

		_sceneManager = std::make_unique<Managers::SceneManager>();
		_assetManager = std::make_unique<Managers::AssetManager>();
	}

	RAINDROP_API Application::~Application(){
		RAINDROP_profile_function();
	}

	RAINDROP_API const char* Application::name() const{
		RAINDROP_profile_function();
		return _name.c_str();
	}

	RAINDROP_API Version Application::version() const{
		RAINDROP_profile_function();
		return _version;
	}

	RAINDROP_API Scene Application::createScene(usize capacity){
		return Scene(_sceneManager->createScene(capacity));
	}

	RAINDROP_API void Application::destroySceen(Scene scene){
		_sceneManager->destroyScene(scene.getScenePtr());
	}
}