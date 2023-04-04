#include "Core/Scene/EntityManager.hpp"

namespace Raindrop::Core::Scene{
	EntityManager::EntityManager(Memory::Allocator& allocator, usize capacity) : _allocator{allocator}, _freeEntities{allocator}{
		RAINDROP_profile_function();
		_capacity = capacity;

		for (usize i=0; i<capacity; i++){
			_freeEntities.push(static_cast<ID32>(i));
		}
	}

	EntityManager::~EntityManager(){
		RAINDROP_profile_function();

	}

	ID32 EntityManager::create(){
		RAINDROP_profile_function();
		auto next = _freeEntities.front();
		if (!next) throw std::overflow_error("overflow entites, cannot create more entites with this scene");
		_freeEntities.pop();
		return *next;
	}

	void EntityManager::destroy(ID32 entity){
		RAINDROP_profile_function();
		#ifndef NDEBUG
			auto curr = _freeEntities.front();
			while (curr){
				if (*curr == entity) throw std::invalid_argument("cannot destroy the entity, the ID is already released");
				curr++;
			}
		#endif

		if (entity >= _capacity) throw std::out_of_range("cannot destroy the entity, the ID is out of the bounding of the manager");

		_freeEntities.push(entity);
	}
}