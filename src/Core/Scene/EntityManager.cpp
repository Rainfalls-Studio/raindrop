#include <Core/Scene/EntityManager.hpp>
#include <Core/Debug/profiler.hpp>

namespace Raindrop::Core::Scene{
	RAINDROP_API EntityManager::EntityManager(usize capacity){
		RAINDROP_profile_function();
		_capacity = capacity;

		for (usize i=0; i<capacity; i++){
			_freeEntities.push(static_cast<EntityID>(i));
		}
	}

	RAINDROP_API EntityManager::~EntityManager(){
		RAINDROP_profile_function();
	}

	RAINDROP_API EntityID EntityManager::create(){
		RAINDROP_profile_function();
		EntityID entity = _freeEntities.front();
		_freeEntities.pop();
		return entity;
	}

	RAINDROP_API void EntityManager::destroy(EntityID entity){
		RAINDROP_profile_function();
		if (entity >= _capacity) throw std::out_of_range("cannot destroy the entity, the ID is out of the bounding of the manager");
		_freeEntities.push(entity);
	}
}