#include <Wrappers/EntityWrapper.hpp>

namespace Raindrop::Wrappers{
	RAINDROP_API EntityWrapper::EntityWrapper() : _ID{INVALID_ENTITY}{}
	RAINDROP_API EntityWrapper::EntityWrapper(ScenePtr scene, EntityID ID) : _scene{scene}, _ID{ID}{}
	RAINDROP_API EntityWrapper::EntityWrapper(const EntityWrapper& other) : _scene{other._scene}, _ID{other._ID}{}
	
	RAINDROP_API EntityWrapper::~EntityWrapper(){}

	RAINDROP_API EntityWrapper& EntityWrapper::operator=(const EntityWrapper& other){
		_scene = other._scene;
		_ID = other._ID;
		return *this;
	}

	RAINDROP_API bool EntityWrapper::hasComponent(const char* name) const{
		return hasComponent(_scene.lock()->getComponentID(name));
	}

	RAINDROP_API bool EntityWrapper::hasComponent(ComponentID ID) const{
		return _scene.lock()->hasComponent(_ID, ID);
	}

	RAINDROP_API bool EntityWrapper::operator==(EntityID ID) const{
		return _ID == ID;
	}

	RAINDROP_API bool EntityWrapper::operator==(const EntityWrapper& other) const{
		return _ID == other._ID && _scene.lock() == other._scene.lock();
	}
	
	RAINDROP_API bool EntityWrapper::operator!=(EntityID ID) const{
		return _ID != ID;
	}

	RAINDROP_API bool EntityWrapper::operator!=(const EntityWrapper& other) const{
		return _ID != other._ID || _scene.lock() != other._scene.lock();
	}
}