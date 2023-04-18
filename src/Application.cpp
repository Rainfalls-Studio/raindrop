#include <Application.hpp>
#include <Core/Debug/profiler.hpp>

namespace Raindrop{
	RAINDROP_API Application::Application(const char* name, Version version, Core::Memory::Allocator& allocator) : _allocator{allocator}{
		RAINDROP_profile_function();
		_name = name;
		_version = version;

		_sceneManager = Core::Memory::allocateNew<Managers::SceneManager>(_allocator, _allocator);
	}

	RAINDROP_API Application::~Application(){
		RAINDROP_profile_function();

		Core::Memory::deallocateDelete(_allocator, *_sceneManager);
	}

	RAINDROP_API const char* Application::name() const{
		RAINDROP_profile_function();
		return _name.str();
	}

	RAINDROP_API Version Application::version() const{
		RAINDROP_profile_function();
		return _version;
	}

	ScenePtr Application::createScene(usize capacity){
		return _sceneManager->createScene(capacity);
	}

	void Application::destroySceen(ScenePtr scene){
		_sceneManager->destroyScene(scene);
	}
}