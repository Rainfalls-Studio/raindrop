#include "SystemManager.hpp"

namespace Raindrop::Core::Scene{
	SystemManager::SystemManager(Memory::Allocator& allocator) : _systems{allocator}{
		RAINDROP_profile_function();
	}

	SystemManager::~SystemManager(){
		RAINDROP_profile_function();
		auto curr = _systems.front();
		while (curr){
			if (curr->_system) Memory::deallocateDelete(curr->_system->_allocator, curr->_system);
			curr++;
		}
	}

	void SystemManager::pushSystem(System* system, Signature signature){
		RAINDROP_profile_function();
		SystemData data;
		data._system = system;
		data._signature = signature;

		_systems.push(data);
	}

	void SystemManager::popSystem(System* system){
		RAINDROP_profile_function();
		auto curr = _systems.front();

		while (curr){
			if (curr->_system == system){
				_systems.remove(curr);
				break;
			}
			curr++;
		}
	}
	
	void SystemManager::entitySignatureUpdate(ID32 entity, Signature oldSignature, Signature signature){
		RAINDROP_profile_function();

		auto curr = _systems.front();

		while (curr){
			auto& sig = curr->_signature;
			auto& system = curr->_system;

			if (!(oldSignature & sig)){
				if (signature & sig){
					system->_entities.pushBack(entity);
				} else {
					removeEntity(system, entity);
				}
			}

			curr++;
		}
	}

	void SystemManager::entityRemoved(ID32 entity, Signature signature){
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

		auto curr = system->_entities.begin();

		while (curr){
			if (curr->_data == entity){
				system->_entities.erase(curr);
				break;
			}
			curr++;
		}
	}
}