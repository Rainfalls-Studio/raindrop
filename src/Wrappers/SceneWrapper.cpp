#include <Wrappers/SceneWrapper.hpp>
#include <Wrappers/EntityWrapper.hpp>

namespace Raindrop::Wrappers{
	RAINDROP_API SceneWrapper::SceneWrapper(Managers::ScenePtr scene) : _scene{scene}{}
	RAINDROP_API SceneWrapper::SceneWrapper(const SceneWrapper& other) : _scene{other._scene}{}

	RAINDROP_API SceneWrapper::~SceneWrapper(){}

	RAINDROP_API SceneWrapper& SceneWrapper::operator=(const SceneWrapper& other){
		_scene = other._scene;
		return *this;
	}

	RAINDROP_API EntityWrapper SceneWrapper::createEntity(){
		return EntityWrapper(_scene, _scene.lock()->createEntity());
	}

	RAINDROP_API void SceneWrapper::destroyEntity(EntityWrapper entity){
		if (auto scene = _scene.lock()){
			scene->destroyEntity(entity._ID);
		} else {
			throw std::runtime_error("the scene is expired");
		}
	}

	RAINDROP_API usize SceneWrapper::capacity(){
		if (auto scene = _scene.lock()){
			return scene->capacity();
		} else {
			throw std::runtime_error("the scene is expired");
		}
	}

	RAINDROP_API ComponentID SceneWrapper::getComponentID(const char* name) const{
		if (auto scene = _scene.lock()){
			return scene->getComponentID(name);
		} else {
			throw std::runtime_error("the scene is expired");
		}
	}

	RAINDROP_API Managers::ScenePtr SceneWrapper::getScenePtr() const{
		return _scene;
	}

	RAINDROP_API void SceneWrapper::_registerComponent(const char* name, usize size, usize alignement){
		if (auto scene = _scene.lock()){
			scene->registerComponent(name, size, alignement);
		} else {
			throw std::runtime_error("the scene is expired");
		}
	}

	RAINDROP_API void SceneWrapper::_removeComponent(const char* name){
		if (auto scene = _scene.lock()){
			scene->removeComponent(name);
		} else {
			throw std::runtime_error("the scene is expired");
		}
	}

	RAINDROP_API Signature SceneWrapper::_getComponentSignature(const char* name) const{
		if (auto scene = _scene.lock()){
			return scene->getComponentSignature(getComponentID(name));
		} else {
			throw std::runtime_error("the scene is expired");
		}
	}

	RAINDROP_API bool SceneWrapper::expired() const{
		return _scene.expired();
	}
}