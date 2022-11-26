/**
 * @brief this file contain the entity component system (ECS)
 * @warning DO NOT CHANGE ANYTHING IF YOU NOT KNOWN WHAT YOU ARE DOING
 * @attention IF YOU DO ANY CHANGE IN THIS FILE, MAKE SURE THAT THE FILES THAT DEPEND FROT THIS ARE RECOMPILED
 * @authors Austin Morlan, some changes by @Aalexdev (aaleex3984@gmail.com), ... (add here)
 */

// =============================================================================
// ECS
// architecture from Austin Morlan : https://austinmorlan.com/posts/entity_component_system/
// =============================================================================

#ifndef __RAINDROP_ECS_HPP__
#define __RAINDROP_ECS_HPP__

#include "core.hpp"

namespace rnd::ECS{
	/**
	 * @brief set the entity and componenent type, which also define the max physical count of entities and component
	 */
	using Entity = std::uint32_t;
	using ComponentType = std::uint8_t;

	static constexpr Entity MAX_ENTITIES = 1024; // thoses values are just exemples and can be change
	static constexpr ComponentType MAX_COMPONENT = 32;

	/**
	 * @brief used to define entites components, for an example, is an entity has Transfoorm (type 1) RigidBody (type 2) and Gravity (type 3) the bitset will be 0b111
	 * each bits represent a component, and the MAX_COMPONENENT delclaration define the lenght of the bitset.
	 * as an example a bitset of lenght 8 will be like 0b00000000 with 8 zeros
	 */
	using Signature = Bitset<MAX_COMPONENT>; 

	/**
	 * @brief the mother class used for any component Array
	 * 
	 */
	class IComponentArray{
		public:
			virtual ~IComponentArray() = default;
			virtual void entityDestroyed(Entity entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray{
		public:
			void insert(Entity entity, T component){
				RND_ASSERT(_entityToIndexMap.find(entity) == _entityToIndexMap.end(), "Component added to same entity more than once.");

				// Put new entry at end and update the maps
				size_t newIndex = _size;
				_entityToIndexMap[entity] = newIndex;
				_indexToEntityMap[newIndex] = entity;
				_componentArray[newIndex] = component;
				++_size;
			}

			void remove(Entity entity){
				RND_ASSERT(_entityToIndexMap.find(entity) != _entityToIndexMap.end(), "Removing non-existent component.");

				// Copy element at end into deleted element's place to maintain density
				size_t indexOfRemovedEntity = _entityToIndexMap[entity];
				size_t indexOfLastElement = _size - 1;
				_componentArray[indexOfRemovedEntity] = _componentArray[indexOfLastElement];

				// Update map to point to moved spot
				Entity entityOfLastElement = _indexToEntityMap[indexOfLastElement];
				_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
				_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

				_entityToIndexMap.erase(entity);
				_indexToEntityMap.erase(indexOfLastElement);

				--_size;
			}

			T& get(Entity entity){
				RND_ASSERT(_entityToIndexMap.find(entity) != _entityToIndexMap.end(), "Retrieving non-existent component.");

				// Return a reference to the entity's component
				return _componentArray[_entityToIndexMap[entity]];
			}

			void entityDestroyed(Entity entity) override{
				if (_entityToIndexMap.find(entity) != _entityToIndexMap.end()){
					// Remove the entity's component if it existed
					remove(entity);
				}
			}
		
		private:
			/**
			 * @brief the packed array (no holes) of generic type T
			 * set the maximal entity count
			 */
			Array<T, MAX_ENTITIES> _componentArray;

			/**
			 * @brief map entity id to array index
			 */
			UnorderedMap<Entity, std::size_t> _entityToIndexMap;

			/**
			 * @brief map array index to entity id
			 * the invers of the _entotyToIndexMap
			 */
			UnorderedMap<std::size_t, Entity> _indexToEntityMap;

			/**
			 * @brief total size of entries in the array
			 */
			std::size_t _size;
	};

	class ComponentManager{
		public:
			template<typename T> 
			void registerComponent(){
				const char* typeName = typeid(T).name();

				RND_ASSERT(_ComponentTypes.find(typeName) == _ComponentTypes.end(), "Registering component type more than once.");

				// Add this component type to the component type map
				_ComponentTypes.insert({typeName, _NextComponentType});

				// Create a ComponentArray pointer and add it to the component arrays map
				_componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

				// Increment the value so that the next component registered will be different
				_NextComponentType++;
			}

			template<typename T>
			ComponentType getComponentType(){
				const char* typeName = typeid(T).name();

				RND_ASSERT(_ComponentTypes.find(typeName) != _ComponentTypes.end(), "Component not registered before use.");

				// Return this component's type - used for creating signatures
				return _ComponentTypes[typeName];
			}

			template<typename T>
			void addComponent(Entity entity, T component){
				// Add a component to the array for an entity
				getComponentArray<T>()->insert(entity, component);
			}

			template<typename T>
			void removeComponent(Entity entity){
				// Remove a component from the array for an entity
				getComponentArray<T>()->remove(entity);
			}

			template<typename T>
			T& getComponent(Entity entity){
				// Get a reference to a component from the array for an entity
				return getComponentArray<T>()->get(entity);
			}

			void entityDestroyed(Entity entity){
				// Notify each component array that an entity has been destroyed
				// If it has a component for that entity, it will remove it
				for (auto const& pair : _componentArrays){
					auto const& component = pair.second;

					component->entityDestroyed(entity);
				}
			}

		private:
			// Map from type string pointer to a component type
			UnorderedMap<const char*, ComponentType> _ComponentTypes{};

			// Map from type string pointer to a component array
			UnorderedMap<const char*, std::shared_ptr<IComponentArray>> _componentArrays{};

			// The component type to be assigned to the next registered component - starting at 0
			ComponentType _NextComponentType{};

			// Convenience function to get the statically casted pointer to the ComponentArray of type T.
			template<typename T> std::shared_ptr<ComponentArray<T>> getComponentArray(){
				const char* typeName = typeid(T).name();
				RND_ASSERT(_ComponentTypes.find(typeName) != _ComponentTypes.end(), "Component not registered before use.");
				return std::static_pointer_cast<ComponentArray<T>>(_componentArrays[typeName]);
			}
	};

	class EntityManager{
		public:
			EntityManager(){
				// fill the available entity queue with ids
				for (Entity entity = 0; entity<MAX_ENTITIES; entity++)
					_availablesEntities.push(entity);
			}

			/**
			 * @brief create an entity and return he's id
			 * @warning need to be destroyed 
			 * @return Entity id
			 */
			Entity create(){
				RND_ASSERT(_livingEntityCount < MAX_ENTITIES, "Too many entites in existence.");

				// take the fisrt available id from the available entity queue
				Entity id = _availablesEntities.front();
				_availablesEntities.pop();
				_livingEntityCount++;

				// return the id
				return id;
			}

			/**
			 * @brief destroy the given entity, it will no more be managed by the Entity Manager
			 * @param entity the entity to destroy
			 */
			void destroy(Entity entity){
				RND_ASSERT(entity < MAX_ENTITIES, "Entiity out of range.");

				// reset the entity's components
				_signatures[entity].reset();

				// and push the new availble entity id into the queue
				_availablesEntities.push(entity);
				_livingEntityCount--;
			}

			/**
			 * @brief Set the Signature of the given entity
			 * @param entity the targeted entity
			 * @param signature the new signature state of the entity
			 */
			void setSignature(Entity entity, Signature signature){
				RND_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
				_signatures[entity] = signature;
			}

			/**
			 * @brief Get the Signature of the given entity
			 * @param entity the targeted entity
			 * @return the signature of the entity as a bitfield with the size of EntityManager::MAX_COMPONENT
			 */
			Signature getSignature(Entity entity){
				RND_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
				return _signatures[entity];
			}

			inline Entity getLivingCount() const {
				return _livingEntityCount;
			}

		private:

			/**
			 * @brief the queue of available entities id to avoid holes in the entities array
			 */
			std::queue<Entity> _availablesEntities{};

			/**
			 * @brief the array who store the entities signature
			 */
			std::array<Signature, MAX_ENTITIES> _signatures{};

			/**
			 * @brief the count of entities handeled by the manager
			 * used to keep a limit about entities count 
			 */
			Entity _livingEntityCount{};
	};

	class System{
		public:
			Set<Entity> mEntities;
	};

	class SystemManager{
		public:
			template<typename T, typename... _Args>
			std::shared_ptr<T> registerSystem(_Args&&... __args) {
				const char* typeName = typeid(T).name();

				RND_ASSERT(mSystems.find(typeName) == mSystems.end(), "Registering system more than once.");

				// Create a pointer to the system and return it so it can be used externally
				auto system = std::make_shared<T>(std::forward<_Args>(__args)...);
				mSystems.insert({typeName, system});
				return system;
			}

			template<typename T>
			void setSignature(Signature signature){
				const char* typeName = typeid(T).name();

				RND_ASSERT(mSystems.find(typeName) != mSystems.end(), "System used before registered.");

				// Set the signature for this system
				mSignatures.insert({typeName, signature});
			}

			void entityDestroyed(Entity entity){
				// Erase a destroyed entity from all system lists
				// mEntities is a set so no check needed
				for (auto const& pair : mSystems)
				{
					auto const& system = pair.second;

					system->mEntities.erase(entity);
				}
			}

			void EntitySignatureChanged(Entity entity, Signature entitySignature){
				// Notify each system that an entity's signature changed
				for (auto const& pair : mSystems)
				{
					auto const& type = pair.first;
					auto const& system = pair.second;
					auto const& systemSignature = mSignatures[type];

					// Entity signature matches system signature - insert into set
					if ((entitySignature & systemSignature) == systemSignature)
					{
						system->mEntities.insert(entity);
					}
					// Entity signature does not match system signature - erase from set
					else
					{
						system->mEntities.erase(entity);
					}
				}
			}

		private:
			// Map from system type string pointer to a signature
			UnorderedMap<const char*, Signature> mSignatures{};

			// Map from system type string pointer to a system pointer
			UnorderedMap<const char*, std::shared_ptr<System>> mSystems{};
	};

	class Registry{
		public:
			Registry() {Init();}

			void Init(){
				// Create pointers to each manager
				_componentManager = std::make_unique<ComponentManager>();
				_entityManager = std::make_unique<EntityManager>();
				_systemManager = std::make_unique<SystemManager>();
			}

			// Entity methods
			Entity create(){
				return _entityManager->create();
			}

			void destroy(Entity entity){
				_entityManager->destroy(entity);

				_componentManager->entityDestroyed(entity);

				_systemManager->entityDestroyed(entity);
			}


			// Component methods
			template<typename T>
			void registerComponent(){
				_componentManager->registerComponent<T>();
			}

			template<typename T>
			void addComponent(Entity entity, T component){
				_componentManager->addComponent<T>(entity, component);

				auto signature = _entityManager->getSignature(entity);
				signature.set(_componentManager->getComponentType<T>(), true);
				_entityManager->setSignature(entity, signature);

				_systemManager->EntitySignatureChanged(entity, signature);
			}

			template<typename T>
			void removeComponent(Entity entity) {
				_componentManager->removeComponent<T>(entity);

				auto signature = _entityManager->getSignature(entity);
				signature.set(_componentManager->getComponentType<T>(), false);
				_entityManager->setSignature(entity, signature);

				_systemManager->EntitySignatureChanged(entity, signature);
			}

			template<typename T>
			T& getComponent(Entity entity){
				return _componentManager->getComponent<T>(entity);
			}

			template<typename T>
			ComponentType getComponentType(){
				return _componentManager->getComponentType<T>();
			}

			// System methods
			template<typename T, typename... _Args>
			std::shared_ptr<T> RegisterSystem(_Args&&... __args){
				return _systemManager->registerSystem<T>(std::forward<_Args>(__args)...);
			}

			template<typename T>
			void setSystemSignature(Signature signature){
				_systemManager->setSignature<T>(signature);
			}

		private:
			std::unique_ptr<ComponentManager> _componentManager;
			std::unique_ptr<EntityManager> _entityManager;
			std::unique_ptr<SystemManager> _systemManager;
	};
}

#endif