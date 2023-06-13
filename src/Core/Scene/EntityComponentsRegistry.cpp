#include <Raindrop/Core/Scene/EntityComponentsRegistry.hpp>


namespace Raindrop::Core::Scene{
	EntityComponentsRegistry::EntityComponentsRegistry(uint32_t entityCount, uint32_t componentCount){
		_ids.resize(componentCount);
		for (auto &v : _ids){
			v.resize(entityCount);
			std::fill(v.begin(), v.end(), INVALID_COMPONENT_HANDLE_ID);
		}
	}

	EntityComponentsRegistry::~EntityComponentsRegistry(){

	}

	ComponentHandleID& EntityComponentsRegistry::get(EntityID entity, ComponentID component){
		return _ids[component][entity];
	}

	const ComponentHandleID& EntityComponentsRegistry::get(EntityID entity, ComponentID component) const{
		return _ids[component][entity];
	}

	void EntityComponentsRegistry::set(EntityID entity, ComponentID component, ComponentHandleID handle){
		_ids[component][entity] = handle;
	}
	
	uint32_t EntityComponentsRegistry::componentCount() const{
		return _ids.size();
	}

	uint32_t EntityComponentsRegistry::entityCount() const{
		return _ids[0].size();
	}
}