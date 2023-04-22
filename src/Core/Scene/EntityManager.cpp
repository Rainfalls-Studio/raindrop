#include "Core/Scene/EntityManager.hpp"

namespace Raindrop::Core::Scene{
	RAINDROP_API EntityManager::EntityManager(Memory::Allocator& allocator, usize capacity) : _allocator{allocator}, _freeEntities{allocator}{
		RAINDROP_profile_function();
		_capacity = capacity;

		for (usize i=0; i<capacity; i++){
			_freeEntities.push(static_cast<ID32>(i));
		}
	}

	RAINDROP_API EntityManager::~EntityManager(){
		RAINDROP_profile_function();
	}

	RAINDROP_API ID32 EntityManager::create(){
		RAINDROP_profile_function();
		auto next = _freeEntities.front();
		if (!next) throw std::overflow_error("overflow entites, cannot create more entites with this scene");
		_freeEntities.pop();
		return *next;
	}

	RAINDROP_API void EntityManager::destroy(ID32 entity){
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

	RAINDROP_API bool EntityManager::exists(ID32 entity) const{
		RAINDROP_profile_function();
		auto& list = const_cast<Memory::List<ID32>&>(_freeEntities);
		auto it = list.front();
		while (it){
			if (*it == entity) return false;
			it++;
		}
		return true;
	}
}