#include "Scene.hpp"

namespace Raindrop::Core::Scene{
	Scene::Scene(Memory::Allocator& allocator, usize capacity, usize componentCount) : _allocator{allocator}, _managers{allocator}, _typeToID{allocator}, _freeComponentIDs{allocator}, _entityManager{allocator, capacity}{
		RAINDROP_profile_function();

		_capacity = capacity;

		for (uint32 i=0; i<componentCount; i++){
			_freeComponentIDs.push(i);
		}

		_managers.resize(componentCount);
		for (usize i=0; i<componentCount; i++){
			_managers[i] = nullptr;
		}
	}

	Scene::~Scene(){
		RAINDROP_profile_function();
		
		for (usize i=0; i<_managers.size(); i++){
			auto& manager = _managers[i];
			if (manager) Memory::deallocateDelete(_allocator, manager);
		}
	}

	ID32 Scene::createEntity(){
		RAINDROP_profile_function();
		return _entityManager.create();
	}

	void Scene::destroyEntity(ID32 entity){
		RAINDROP_profile_function();
		_entityManager.destroy(entity);
	}

	void Scene::registerComponent(usize typeId, usize typeSize, usize typeAlignement){
		RAINDROP_profile_function();
		RAINDROP_assert(_typeToID.has(typeId));

		ID32 id = *_freeComponentIDs.front();
		_freeComponentIDs.pop();

		_typeToID.insert(typeId, id);
		
		_managers[id] = Memory::allocateNew<ComponentManager>(_allocator, _allocator, typeSize, typeAlignement, _capacity);
	}

	void Scene::removeComponent(usize typeId){
		RAINDROP_profile_function();
		RAINDROP_assert(_typeToID.has(typeId));

		ID32 id = *_typeToID.get(typeId);
		Memory::deallocateDelete(_allocator, _managers[id]);
		_managers[id] = nullptr;

		_freeComponentIDs.push(id);
		_typeToID.remove(typeId);
	}

	void* Scene::getComponent(ID32 entity, usize typeId){
		RAINDROP_profile_function();
		RAINDROP_assert(_typeToID.has(typeId));

		ID32 id = *_typeToID.get(typeId);
		return _managers[id]->get(entity);
	}

	void Scene::setComponent(ID32 entity, usize typeId, void* component){
		RAINDROP_profile_function();
		RAINDROP_assert(_typeToID.has(typeId));

		ID32 id = *_typeToID.get(typeId);
		_managers[id]->set(entity, component);
	}
}