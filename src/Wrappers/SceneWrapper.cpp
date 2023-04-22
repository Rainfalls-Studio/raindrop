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
		return EntityWrapper(_scene, _scene->createEntity());
	}

	RAINDROP_API void SceneWrapper::destroyEntity(EntityWrapper entity){
		_scene->destroyEntity(entity._ID);
	}

	RAINDROP_API usize SceneWrapper::capacity(){
		return _scene->capacity();
	}

	RAINDROP_API ComponentID SceneWrapper::getComponentID(const char* name) const{
		return _scene->getComponentID(name);
	}

	RAINDROP_API Managers::ScenePtr SceneWrapper::getScenePtr() const{
		return _scene;
	}

	RAINDROP_API void SceneWrapper::_registerComponent(const char* name, usize size, usize alignement){
		_scene->registerComponent(name, size, alignement);
	}

	RAINDROP_API void SceneWrapper::_removeComponent(const char* name){
		_scene->removeComponent(name);
	}

	RAINDROP_API Signature SceneWrapper::_getComponentSignature(const char* name) const{
		return _scene->getComponentSignature(getComponentID(name));
	}
}