#include <Core/Scene/Scene.hpp>
#include <Core/Debug/profiler.hpp>

namespace Raindrop::Core::Scene{
	RAINDROP_API Scene::Scene(usize capacity) : _entityManager{capacity}, _entitySignatures{capacity}{
		RAINDROP_profile_function();

		_capacity = capacity;

		for (uint32 i=0; i<MAX_COMPONENT_COUNT; i++){
			_freeComponentIDs.push(i);
		}

		for (usize i=0; i<MAX_COMPONENT_COUNT; i++){
			_componentManagers[i] = nullptr;
		}
	}

	RAINDROP_API Scene::~Scene(){
		RAINDROP_profile_function();
	}

	RAINDROP_API ID32 Scene::createEntity(){
		RAINDROP_profile_function();
		ID32 entity = _entityManager.create();
		_entitySignatures.get(entity).clear();
		return entity;
	}

	RAINDROP_API void Scene::destroyEntity(ID32 entity){
		RAINDROP_profile_function();
		if (!isIDValid(entity)) throw std::invalid_argument("cannot destroy an invalid entity");

		_entityManager.destroy(entity);
		Signature signature = _entitySignatures.get(entity);
		_systemManager.entityRemoved(entity, signature);
	}


	RAINDROP_API void Scene::registerComponent(const char* name, usize typeSize, usize typeAlignement){
		RAINDROP_profile_function();
		if (_nameToID.find(name) != _nameToID.end()) throw std::invalid_argument("cannot register an already registred component");

		ID32 id = _freeComponentIDs.front();
		_freeComponentIDs.pop();

		_nameToID.insert({name, id});
		_componentManagers[id] = std::make_unique<ComponentManager>(typeSize, typeAlignement, _capacity);
	}

	RAINDROP_API void Scene::removeComponent(const char* name){
		RAINDROP_profile_function();
		if (_nameToID.find(name) == _nameToID.end()) throw std::invalid_argument("cannot remove a non registred component");

		ID32& id = _nameToID[name];
		_componentManagers[id].release();

		_freeComponentIDs.push(id);
		_nameToID.erase(name);
	}

	RAINDROP_API void* Scene::getComponent(ID32 entity, ID32 componentID){
		RAINDROP_profile_function();
		if (isIDValid(componentID)) throw std::invalid_argument("invalid component ID");
		if (isIDValid(entity)) throw std::invalid_argument("invalid entity ID");
		if (!hasComponent(entity, componentID)) throw std::runtime_error("the entity does not have this component");

		return _componentManagers[componentID]->get(entity);
	}

	RAINDROP_API void Scene::setComponent(ID32 entity, ID32 componentID, void* component){
		RAINDROP_profile_function();
		if (isIDValid(componentID)) throw std::invalid_argument("invalid component ID");
		if (isIDValid(entity)) throw std::invalid_argument("invalid entity ID");
		_componentManagers[componentID]->set(entity, component);
	}

	RAINDROP_API bool Scene::hasComponent(ID32 entity, ID32 componentID) const{
		RAINDROP_profile_function();
		if (isIDValid(componentID)) throw std::invalid_argument("invalid component ID");
		if (isIDValid(entity)) throw std::invalid_argument("invalid entity ID");

		return _entitySignatures.getBit(entity, componentID);
	}

	RAINDROP_API void Scene::addComponent(ID32 entity, ID32 componentID, void* component){
		RAINDROP_profile_function();
		if (isIDValid(componentID)) throw std::invalid_argument("invalid component ID");
		if (isIDValid(entity)) throw std::invalid_argument("invalid entity ID");

		if (component){
			_componentManagers[componentID]->set(entity, component);
		}

		Signature oldSignature = _entitySignatures.get(entity);
		_entitySignatures.setBit(entity, componentID, true);
		Signature newSignature = _entitySignatures.get(entity);

		_systemManager.entitySignatureUpdate(entity, oldSignature, newSignature);
	}

	RAINDROP_API void Scene::eraseComponent(ID32 entity, ID32 componentID){
		RAINDROP_profile_function();
		if (isIDValid(componentID)) throw std::invalid_argument("invalid component ID");
		if (isIDValid(entity)) throw std::invalid_argument("invalid entity ID");

		Signature oldSignature = _entitySignatures.get(entity);
		_entitySignatures.setBit(entity, componentID, false);
		Signature newSignature = _entitySignatures.get(entity);

		_systemManager.entitySignatureUpdate(entity, oldSignature, newSignature);
	}

	RAINDROP_API void Scene::pushSystem(std::shared_ptr<System> system, Signature signature){
		RAINDROP_profile_function();
		_systemManager.pushSystem(system, signature);
	}

	RAINDROP_API void Scene::popSystem(std::shared_ptr<System> system){
		RAINDROP_profile_function();
		_systemManager.popSystem(system);
	}

	RAINDROP_API Signature Scene::getComponentSignature(ID32 componentID) const{
		Signature signature{};
		if (isIDValid(componentID)) throw std::invalid_argument("invalid component ID");
		signature.set(componentID, true);
		return signature;
	}

	RAINDROP_API usize Scene::capacity() const{
		return _capacity;
	}

	RAINDROP_API ID32 Scene::getComponentID(const char* name) const{
		return _nameToID.find(name) != _nameToID.end();
	}

	RAINDROP_API bool Scene::isIDValid(ID32 id){
		return id != INVALID_ENTITY;
	}
}