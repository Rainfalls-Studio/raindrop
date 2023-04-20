#include <Wrappers/SceneWrapper.hpp>
#include <Wrappers/EntityWrapper.hpp>

namespace Raindrop::Wrappers{
	SceneWrapper::SceneWrapper(Managers::ScenePtr scene) : _scene{scene}{}
	SceneWrapper::SceneWrapper(const SceneWrapper& other) : _scene{other._scene}{}

	SceneWrapper::~SceneWrapper(){}

	SceneWrapper& SceneWrapper::operator=(const SceneWrapper& other){
		_scene = other._scene;
		return *this;
	}

	EntityWrapper SceneWrapper::createEntity(){
		return EntityWrapper(_scene, _scene->createEntity());
	}

	void SceneWrapper::destroyEntity(EntityWrapper entity){
		_scene->destroyEntity(entity._ID);
	}

	usize SceneWrapper::capacity(){
		return _scene->capacity();
	}

	ComponentID SceneWrapper::getComponentID(const char* name) const{
		return _scene->getComponentID(name);
	}

	Managers::ScenePtr SceneWrapper::getScenePtr() const{
		return _scene;
	}

	void SceneWrapper::_registerComponent(const char* name, usize size, usize alignement){
		_scene->registerComponent(name, size, alignement);
	}

	void SceneWrapper::_removeComponent(const char* name){
		_scene->removeComponent(name);
	}

	Signature SceneWrapper::_getComponentSignature(const char* name) const{
		return _scene->getComponentSignature(getComponentID(name));
	}
}