#include <Core/Scene/SystemManager.hpp>
#include <Core/Debug/profiler.hpp>

namespace Raindrop::Core::Scene{
	RAINDROP_API SystemManager::SystemManager(){
		RAINDROP_profile_function();
	}

	RAINDROP_API SystemManager::~SystemManager(){
		RAINDROP_profile_function();
	}

	RAINDROP_API void SystemManager::pushSystem(std::shared_ptr<System> system, Signature signature){
		RAINDROP_profile_function();
		if (!system) throw std::invalid_argument("cannot push a null system");

		SystemData data;
		data._system = system;
		data._signature = signature;

		_systems.push_back(data);
	}

	RAINDROP_API void SystemManager::popSystem(std::shared_ptr<System> system){
		RAINDROP_profile_function();
		if (!system) throw std::invalid_argument("cannot pop a null system");
		_systems.erase(std::find(_systems.begin(), _systems.end(), system));
	}
	
	RAINDROP_API void SystemManager::entitySignatureUpdate(ID32 entity, Signature oldSignature, Signature signature){
		RAINDROP_profile_function();

		for (auto &sysData : _systems){
			auto& sig = sysData._signature;
			auto& sys = sysData._system;

			if (!(oldSignature & sig)){
				if (signature & sig){
					sys->_entities.push_back(entity);
					sys->onEntityCreated(entity);
				}
			} else if (!(signature & sig)){
				removeEntity(sys.get(), entity);
			}
		}
	}

	void RAINDROP_API SystemManager::entityRemoved(ID32 entity, Signature signature){
		RAINDROP_profile_function();

		for (auto& sysData : _systems){
			auto& sig = sysData._signature;
			auto& sys = sysData._system;

			if (signature & sig){
				removeEntity(sys.get(), entity);
			}
		}
	}

	void SystemManager::removeEntity(System* system, ID32 entity){
		RAINDROP_profile_function();
		if (!system) throw std::invalid_argument("cannot remove an entity from a non existante system");

		auto& entities = system->_entities;
		entities.remove(entity);
	}
}