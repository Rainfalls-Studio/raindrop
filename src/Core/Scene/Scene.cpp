#include "Core/Scene/Scene.hpp"
#include <typeinfo>

namespace Raindrop::Core::Scene{
	RAINDROP_API Scene::Scene(Memory::Allocator& allocator, usize capacity) : _allocator{allocator}, _nameToID{allocator}, _freeComponentIDs{allocator}, _entityManager{allocator, capacity}, _entitySignatures{allocator, capacity}, _systemManager{allocator}{
		RAINDROP_profile_function();

		_capacity = capacity;

		for (uint32 i=0; i<MAX_COMPONENT_COUNT; i++){
			_freeComponentIDs.push(i);
		}

		for (usize i=0; i<MAX_COMPONENT_COUNT; i++){
			_componentManagers[i] = nullptr;
		}
	}

	RAINDROP_API Scene::~Scene(){
		RAINDROP_profile_function();

		deleteSystems();
		
		for (usize i=0; i<MAX_COMPONENT_COUNT; i++){
			auto& manager = _componentManagers[i];
			if (manager) Memory::deallocateDelete(_allocator, *manager);
		}
	}

	RAINDROP_API void Scene::deleteSystems(){
		RAINDROP_profile_function();
		auto it = _systemManager._systems.front();
		while (it){
			Memory::deallocateDelete(_allocator, *it->_system);
			it++;
		}
	}

	RAINDROP_API ID32 Scene::createEntity(){
		RAINDROP_profile_function();
		ID32 entity = _entityManager.create();
		_entitySignatures.get(entity).clear();
		return entity;
	}

	RAINDROP_API void Scene::destroyEntity(ID32 entity){
		RAINDROP_profile_function();
		RAINDROP_assert(isIDValid(entity));
		_entityManager.destroy(entity);
		Signature signature = _entitySignatures.get(entity);
		_systemManager.entityRemoved(entity, signature);
	}

	RAINDROP_API bool Scene::exists(ID32 entity) const{
		RAINDROP_profile_function();
		return _entityManager.exists(entity);
	}


	RAINDROP_API void Scene::registerComponent(const char* name, usize typeSize, usize typeAlignement){
		RAINDROP_profile_function();
		RAINDROP_assert(!_nameToID.has(name) && "cannot register an already registred component");

		ID32 id = *_freeComponentIDs.front();
		_freeComponentIDs.pop();

		_nameToID.insert(name, id);
		
		_componentManagers[id] = Memory::allocateNew<ComponentManager>(_allocator, _allocator, typeSize, typeAlignement, _capacity);
	}

	RAINDROP_API void Scene::removeComponent(const char* name){
		RAINDROP_profile_function();
		RAINDROP_assert(_nameToID.has(name) && "cannot remove a non registred component");

		const ID32& id = _nameToID.lookup(name);
		Memory::deallocateDelete(_allocator, _componentManagers[id]);
		_componentManagers[id] = nullptr;

		_freeComponentIDs.push(id);
		_nameToID.remove(name);
	}

	RAINDROP_API void* Scene::getComponent(ID32 entity, ID32 componentID){
		RAINDROP_profile_function();
		RAINDROP_assert(isIDValid(componentID));
		RAINDROP_assert(isIDValid(entity));
		RAINDROP_assert(hasComponent(entity, componentID));

		return _componentManagers[componentID]->get(entity);
	}

	RAINDROP_API void Scene::setComponent(ID32 entity, ID32 componentID, void* component){
		RAINDROP_profile_function();
		RAINDROP_assert(isIDValid(componentID));
		RAINDROP_assert(isIDValid(entity));
		_componentManagers[componentID]->set(entity, component);
	}

	RAINDROP_API bool Scene::hasComponent(ID32 entity, ID32 componentID) const{
		RAINDROP_profile_function();
		RAINDROP_assert(isIDValid(componentID));
		RAINDROP_assert(isIDValid(entity));

		return _entitySignatures.getBit(entity, componentID);
	}

	RAINDROP_API void Scene::addComponent(ID32 entity, ID32 componentID, void* component){
		RAINDROP_profile_function();
		RAINDROP_assert(isIDValid(componentID));
		RAINDROP_assert(isIDValid(entity));

		if (component){
			_componentManagers[componentID]->set(entity, component);
		}

		Signature oldSignature = _entitySignatures.get(entity);
		_entitySignatures.setBit(entity, componentID, true);
		Signature newSignature = _entitySignatures.get(entity);

		_systemManager.entitySignatureUpdate(entity, oldSignature, newSignature);
	}

	RAINDROP_API void Scene::eraseComponent(ID32 entity, ID32 componentID){
		RAINDROP_profile_function();
		RAINDROP_assert(isIDValid(componentID));
		Signature oldSignature = _entitySignatures.get(entity);
		_entitySignatures.setBit(entity, componentID, false);
		Signature newSignature = _entitySignatures.get(entity);

		_systemManager.entitySignatureUpdate(entity, oldSignature, newSignature);
	}

	RAINDROP_API void Scene::pushSystem(System* system, Signature signature){
		RAINDROP_profile_function();
		_systemManager.pushSystem(system, signature);
	}

	RAINDROP_API void Scene::popSystem(System* system){
		RAINDROP_profile_function();
		_systemManager.popSystem(system);
	}

	RAINDROP_API Signature Scene::getComponentSignature(ID32 componentID) const{
		Signature signature{};
		RAINDROP_assert(isIDValid(componentID));
		signature.set(componentID, true);
		return signature;
	}

	RAINDROP_API usize Scene::capacity() const{
		return _capacity;
	}

	RAINDROP_API ID32 Scene::getComponentID(const char* name) const{
		return _nameToID.lookup(name);
	}

	RAINDROP_API bool Scene::isIDValid(ID32 id){
		return id != INVALID_ENTITY;
	}
}