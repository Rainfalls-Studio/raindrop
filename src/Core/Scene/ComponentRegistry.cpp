#include <Raindrop/Core/Scene/ComponentRegistry.hpp>
#include <Raindrop/Core/Scene/ComponentManager.hpp>

namespace Raindrop::Core::Scene{
	ComponentRegistry::ComponentRegistry(uint32_t size){
		_managers.resize(size);

		for (uint32_t i=0; i<size; i++){
			_IDsPool.push(i);
		}
	}

	ComponentRegistry::~ComponentRegistry(){

	}

	ComponentID ComponentRegistry::registerComponent(uint32_t componentSize, size_t typeID, uint32_t size, ConstructorPtr constructor, DestructorPtr destructor){
		ComponentID id = getNextID();
		_managers[id] = std::make_unique<ComponentManager>(componentSize, typeID, size, constructor, destructor);
		_typeIDtoComponentID[typeID] = id;
		return id;
	}

	ComponentID ComponentRegistry::getNextID(){
		if (_IDsPool.empty()){
			CLOG(ERROR, "Engine.Core.Scene") << "Cannot add another component ! The component registry is full !";
			throw std::runtime_error("Cannot add another component !");
		}
		ComponentID id = _IDsPool.front();
		_IDsPool.pop();
		return id;
	}

	void* ComponentRegistry::getComponent(ComponentID ID, ComponentHandleID handleID){
		return _managers[ID]->get(handleID);
	}

	void ComponentRegistry::unregisterComponent(ComponentID id){
		_managers[id].reset();
		_typeIDtoComponentID.erase(id);
		_IDsPool.push(id);
	}
	
	uint32_t ComponentRegistry::size() const{
		return _managers.size();
	}
	
	uint32_t ComponentRegistry::usedSize() const{
		return size() - _IDsPool.size();
	}

	ComponentID ComponentRegistry::getComponentID(size_t typeID){
		return _typeIDtoComponentID[typeID];
	}
	
	ComponentHandleID ComponentRegistry::createHandle(ComponentID component, EntityID entity){
		auto& manager = _managers[component];
		ComponentHandleID handle = manager->createComponent();
		manager->addEntity(entity);
		return handle;
	}

	void ComponentRegistry::destroyHandle(ComponentID component, ComponentHandleID handle, EntityID entity){
		auto& manager = _managers[component];
		manager->destroyComponent(handle);
		manager->removeEntity(entity);
	}

	bool ComponentRegistry::registred(size_t typeID){
		return _typeIDtoComponentID.find(typeID) != _typeIDtoComponentID.end();
	}

	ComponentManager* ComponentRegistry::getManager(ComponentID component){
		return _managers[component].get();
	}
}