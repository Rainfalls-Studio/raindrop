#ifndef __RAINDROP_CORE_SCENE_SCENE_HPP__
#define __RAINDROP_CORE_SCENE_SCENE_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class Scene{
		public:
			Scene(uint32_t entityCount, uint32_t componentCount);
			~Scene();

			uint32_t maxEntityCount() const;
			uint32_t maxComponentCount() const;
			uint32_t usedEntityCount() const;
			uint32_t usedComponentCount() const;

			ComponentID registerComponent(uint32_t componentSize, size_t typeID, uint32_t maxCount, ConstructorPtr constructor, DestructorPtr destructor);
			void unregisterComponent(ComponentID id);
			ComponentID getComponentID(size_t typeID);
			bool componentRegistred(size_t typeID);

			EntityID createEntity();
			void destroyEntity(EntityID ID);

			bool hasComponent(EntityID entity, ComponentID component);
			void* getComponent(EntityID entity, ComponentID component);
			void* createComponent(EntityID entity, ComponentID component);
			void destroyComponent(EntityID entity, ComponentID component);

			template<typename T>
			ComponentID getComponentID(){
				return getComponentID(typeid(T).hash_code());
			}

			template<typename T>
			ComponentID registerComponent(uint32_t maxCount){
				return registerComponent(sizeof(T), typeid(T).hash_code(), maxCount, [](void* component){new (component) T();}, [](void* component){reinterpret_cast<T*>(component)->~T();});
			}

			template<typename T>
			void unregisterComponent(){
				unregisterComponent(getComponentID<T>());
			}

			template<typename T>
			bool componentRegistred(){
				return componentRegistred(getComponentID<T>());
			}

			template<typename T>
			bool hasComponent(EntityID entity){
				return hasComponent(entity, getComponentID<T>());
			}

			template<typename T>
			T& getComponent(EntityID entity){
				return *reinterpret_cast<T*>(getComponent(entity, getComponentID<T>()));
			}

			template<typename T>
			T& createComponent(EntityID entity){
				return *reinterpret_cast<T*>(createComponent(entity, getComponentID<T>()));
			}

			template<typename T>
			void destroyComponent(EntityID entity){
				destroyComponent(entity, getComponentID<T>());
			}

			EntityID root() const;
			EntityID UI(EntityID selectedEntity = INVALID_ENTITY_ID);
			void componentsUI(EntityID entity);

		private:
			std::unique_ptr<ComponentRegistry> _componentRegistry;
			std::unique_ptr<EntityComponentsRegistry> _entityComponentsRegistry;
			std::unique_ptr<EntityManager> _entityManager;

			EntityID _root;
	};
}

#endif