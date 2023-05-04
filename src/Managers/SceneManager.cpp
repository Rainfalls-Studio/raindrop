#include <Managers/SceneManager.hpp>
#include <Core/Debug/profiler.hpp>

namespace Raindrop::Managers{
	RAINDROP_API SceneManager::SceneManager(){
		RAINDROP_profile_function();
	}

	RAINDROP_API SceneManager::~SceneManager(){
		RAINDROP_profile_function();
		_scenes.clear();
	}

	RAINDROP_API ScenePtr SceneManager::createScene(usize capacity){
		RAINDROP_profile_function();
		SceneData data;

		data._scene = std::make_shared<Scene>(capacity);

		_scenes.push_back(data);
		return data._scene;
	}

	RAINDROP_API void SceneManager::destroyScene(ScenePtr scene){
		RAINDROP_profile_function();
		if (scene.expired()) throw std::invalid_argument("cannot destroy this scene. In valid scene");
		_scenes.erase(std::find(_scenes.begin(), _scenes.end(), scene));
	}
}