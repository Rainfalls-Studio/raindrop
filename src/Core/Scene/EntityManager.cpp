#include "EntityManager.hpp"

namespace Raindrop::Core::Scene{
	EntityManager::EntityManager(Memory::Allocator& allocator, usize capacity) : _allocator{allocator}, _freeEntities{allocator}{
		_capacity = capacity;

		for (usize i=0; i<capacity; i++){
			_freeEntities.push(static_cast<ID32>(i));
		}
	}

	EntityManager::~EntityManager(){

	}

	ID32 EntityManager::create(){
		ID32 next = *_freeEntities.front();
		_freeEntities.pop();
		return next;
	}

	void EntityManager::destroy(ID32 entity){
		_freeEntities.push(entity);
	}
}