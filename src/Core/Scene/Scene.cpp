#include "Scene.hpp"

namespace Raindrop::Core::Scene{
	Scene::Scene(Memory::Allocator& allocator, usize capacity) : _allocator{allocator}, _typeToID{allocator}, _freeComponentIDs{allocator}, _entityManager{allocator, capacity}, _entitySignatures{allocator, capacity}, _systemManager{allocator}{
		RAINDROP_profile_function();

		_capacity = capacity;

		for (uint32 i=0; i<MAX_COMPONENT_COUNT; i++){
			_freeComponentIDs.push(i);
		}

		for (usize i=0; i<MAX_COMPONENT_COUNT; i++){
			_componentManagers[i] = nullptr;
		}
	}

	Scene::~Scene(){
		RAINDROP_profile_function();
		
		for (usize i=0; i<MAX_COMPONENT_COUNT; i++){
			auto& manager = _componentManagers[i];
			if (manager) Memory::deallocateDelete(_allocator, *manager);
		}
	}

	ID32 Scene::createEntity(){
		RAINDROP_profile_function();
		ID32 entity = _entityManager.create();
		_entitySignatures.get(entity).clear();
		return entity;
	}

	void Scene::destroyEntity(ID32 entity){
		RAINDROP_profile_function();
		_entityManager.destroy(entity);
		Signature signature = _entitySignatures.get(entity);
		_systemManager.entityRemoved(entity, signature);
	}

	void Scene::registerComponent(usize typeId, usize typeSize, usize typeAlignement){
		RAINDROP_profile_function();
		RAINDROP_assert(!_typeToID.has(typeId) && "cannot register an already registred component");

		ID32 id = *_freeComponentIDs.front();
		_freeComponentIDs.pop();

		_typeToID.insert(typeId, id);
		
		_componentManagers[id] = Memory::allocateNew<ComponentManager>(_allocator, _allocator, typeSize, typeAlignement, _capacity);

	}

	void Scene::removeComponent(usize typeId){
		RAINDROP_profile_function();
		RAINDROP_assert(_typeToID.has(typeId) && "cannot remove a non registred component");

		ID32 id = *_typeToID.get(typeId);
		Memory::deallocateDelete(_allocator, _componentManagers[id]);
		_componentManagers[id] = nullptr;

		_freeComponentIDs.push(id);
		_typeToID.remove(typeId);
	}

	void* Scene::getComponent(ID32 entity, usize typeId){
		RAINDROP_profile_function();
		RAINDROP_assert(_typeToID.has(typeId) && "cannot get a non registred component");
		RAINDROP_assert(hasComponent(entity, typeId));

		ID32 id = *_typeToID.get(typeId);
		return _componentManagers[id]->get(entity);
	}

	void Scene::setComponent(ID32 entity, usize typeId, void* component){
		RAINDROP_profile_function();
		RAINDROP_assert(_typeToID.has(typeId));

		ID32 id = _typeToID.lookup(typeId);
		_componentManagers[id]->set(entity, component);
	}

	bool Scene::hasComponent(ID32 entity, usize componentTypeId) const{
		RAINDROP_profile_function();
		ID32 id = _typeToID.lookup(componentTypeId);
		return _entitySignatures.getBit(entity, id);
	}

	void Scene::addComponent(ID32 entity, usize componentTypeId, void* component){
		RAINDROP_profile_function();
		ID32 id = _typeToID.lookup(componentTypeId);
		if (component){
			_componentManagers[id]->set(entity, component);
		}

		Signature oldSignature = _entitySignatures.get(entity);
		_entitySignatures.setBit(entity, id, true);
		Signature newSignature = _entitySignatures.get(entity);

		_systemManager.entitySignatureUpdate(entity, oldSignature, newSignature);
	}

	void Scene::eraseComponent(ID32 entity, usize componentTypeId){
		RAINDROP_profile_function();
		ID32 id = _typeToID.lookup(componentTypeId);
		Signature oldSignature = _entitySignatures.get(entity);
		_entitySignatures.setBit(entity, id, false);
		Signature newSignature = _entitySignatures.get(entity);

		_systemManager.entitySignatureUpdate(entity, oldSignature, newSignature);
	}

	void Scene::pushSystem(System* system, Signature signature){
		RAINDROP_profile_function();
		_systemManager.pushSystem(system, signature);
	}

	void Scene::popSystem(System* system){
		RAINDROP_profile_function();
		_systemManager.popSystem(system);
	}

	Signature Scene::getComponentSignature(usize typeId) const{
		Signature signature{};
		ID32 id = _typeToID.lookup(typeId);
		signature.set(id, true);
		return signature;
	}
}