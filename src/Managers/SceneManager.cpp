#include <Managers/SceneManager.hpp>

namespace Raindrop::Managers{
	SceneManager::SceneManager(Core::Memory::Allocator& allocator) : _allocator{allocator}{
		RAINDROP_profile_function();
	}

	SceneManager::~SceneManager(){
		RAINDROP_profile_function();
		auto it = _scenes.front();

		// remove remaining undestroyed scenes
		while (it){
			ScenePtr scene = *it;
			_scenes.remove(it);
			Core::Memory::deallocateDelete(_allocator, static_cast<Core::Scene::Scene&>(*scene));
			it++;
		}
	}

	ScenePtr SceneManager::createScene(usize capacity){
		RAINDROP_profile_function();
		ScenePtr scene = Core::Memory::allocateNew<Core::Scene::Scene>(_allocator, _allocator, capacity);
		_scenes.push(scene);
		return static_cast<ScenePtr>(scene);
	}

	void SceneManager::destroyScene(ScenePtr scene){
		RAINDROP_profile_function();
		if (scene == INVALID_SCENE_PTR) throw std::invalid_argument("cannot destroy this scene. In valid scene");

		removeScene(scene);
		Core::Memory::deallocateDelete(_allocator, static_cast<Core::Scene::Scene&>(*scene));
	}

	void SceneManager::removeScene(ScenePtr scene){
		RAINDROP_profile_function();
		auto it = _scenes.front();
		while (it){
			if (*it == scene){
				_scenes.remove(it);
				break;
			}
			it++;
		}
	}
} 