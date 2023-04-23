#include <Application.hpp>
#include <Core/Debug/profiler.hpp>

namespace Raindrop{
	RAINDROP_API Application::Application(const char* name, Version version, Core::Memory::Allocator& allocator) : _allocator{allocator}{
		RAINDROP_profile_function();
		_name = name;
		_version = version;

		_sceneManager = Core::Memory::allocateNew<Managers::SceneManager>(_allocator, _allocator);
		_assetManager = Core::Memory::allocateNew<Managers::AssetManager>(_allocator, _allocator);
	}

	RAINDROP_API Application::~Application(){
		RAINDROP_profile_function();

		Core::Memory::deallocateDelete(_allocator, *_sceneManager);
		Core::Memory::deallocateDelete(_allocator, *_assetManager);
	}

	RAINDROP_API const char* Application::name() const{
		RAINDROP_profile_function();
		return _name.str();
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