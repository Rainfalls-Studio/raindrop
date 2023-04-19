#include <Wrappers/EntityWrapper.hpp>

namespace Raindrop::Wrappers{
	EntityWrapper::EntityWrapper() : _scene{nullptr}, _ID{INVALID_ENTITY}{}
	EntityWrapper::EntityWrapper(Scene* scene, EntityID ID) : _scene{scene}, _ID{ID}{}
	EntityWrapper::EntityWrapper(const EntityWrapper& other) : _scene{other._scene}, _ID{other._ID}{}
	
	EntityWrapper::~EntityWrapper(){}

	EntityWrapper& EntityWrapper::operator=(const EntityWrapper& other){
		_scene = other._scene;
		_ID = other._ID;
		return *this;
	}

	bool EntityWrapper::exists() const{
		return _scene->exists(_ID);
	}

	bool EntityWrapper::hasComponent(const char* name) const{
		return hasComponent(_scene->getComponentID(name));
	}

	bool EntityWrapper::hasComponent(ComponentID ID) const{
		return _scene->hasComponent(_ID, ID);
	}

	bool EntityWrapper::operator==(EntityID ID) const{
		return _ID == ID;
	}

	bool EntityWrapper::operator==(const EntityWrapper& other) const{
		return _ID == other._ID && _scene == other._scene;
	}
	
	bool EntityWrapper::operator!=(EntityID ID) const{
		return _ID != ID;
	}

	bool EntityWrapper::operator!=(const EntityWrapper& other) const{
		return _ID != other._ID || _scene != other._scene;
	}
}