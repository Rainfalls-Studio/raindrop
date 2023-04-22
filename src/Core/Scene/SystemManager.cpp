#include "Core/Scene/SystemManager.hpp"

namespace Raindrop::Core::Scene{
	RAINDROP_API SystemManager::SystemManager(Memory::Allocator& allocator) : _systems{allocator}{
		RAINDROP_profile_function();
	}

	RAINDROP_API SystemManager::~SystemManager(){
		RAINDROP_profile_function();
	}

	RAINDROP_API void SystemManager::pushSystem(System* system, Signature signature){
		RAINDROP_profile_function();
		if (!system) throw std::invalid_argument("cannot push a null system");
		SystemData data;
		data._system = system;
		data._signature = signature;

		_systems.push(data);
	}

	RAINDROP_API void SystemManager::popSystem(System* system){
		RAINDROP_profile_function();
		if (!system) throw std::invalid_argument("cannot pop a null system");
		auto curr = _systems.front();

		while (curr){
			if (curr->_system == system){
				_systems.remove(curr);
				return;
			}
			curr++;
		}

		throw std::invalid_argument("cannot pop a non regsitred system");
	}
	
	RAINDROP_API void SystemManager::entitySignatureUpdate(ID32 entity, Signature oldSignature, Signature signature){
		RAINDROP_profile_function();

		auto curr = _systems.front();

		while (curr){
			auto& sig = curr->_signature;
			auto& system = curr->_system;

			if (!(oldSignature & sig)){
				if (signature & sig){
					system->_entities.pushBack(entity);
					system->onEntityCreated(entity);
				}
			} else if (!(signature & sig)){
				removeEntity(system, entity);
			}

			curr++;
		}
	}

	void RAINDROP_API SystemManager::entityRemoved(ID32 entity, Signature signature){
		RAINDROP_profile_function();

		auto curr = _systems.front();
		while (curr){
			auto& sig = curr->_signature;
			auto& system = curr->_system;

			if (signature & sig){
				removeEntity(system, entity);
			}
			
			curr++;
		}
	}

	void SystemManager::removeEntity(System* system, ID32 entity){
		RAINDROP_profile_function();
		if (!system) throw std::invalid_argument("cannot remove an entity from a non existante system");

		auto curr = system->_entities.begin();

		while (curr){
			if (curr->_data == entity){
				system->_entities.erase(curr);
				system->onEntityDestroyed(entity);
				break;
			}
			curr++;
		}
	}
}